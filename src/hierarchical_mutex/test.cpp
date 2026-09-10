// Tests for hierarchical_mutex (Williams, Listing 3.8).
//
// Include your implementation here once written, e.g.:
//   #include "hierarchical_mutex.hpp"

#include <gtest/gtest.h>
#include <atomic>
#include <mutex>
#include <stdexcept>
#include <thread>
#include "hierarchical_mutex.hpp"

// ---------------------------------------------------------------------------
// Test 1: Correct ordering in a single thread — high → low → OK, no throw.
//
// Construct two hierarchical_mutex instances h_high (e.g. 10000) and
// h_low (e.g. 100). Lock h_high, then lock h_low, then unlock in reverse.
// Nothing should throw.
// ---------------------------------------------------------------------------
TEST(HierarchicalMutex, DescendingOrderSucceeds) {
  HierarchicalMutex high{10000};
  HierarchicalMutex low{100};

  high.lock();
  low.lock();

  low.unlock();
  high.unlock();
}

// ---------------------------------------------------------------------------
// Test 2: Violating ordering in a single thread — low → high → throws.
//
// Lock h_low first, then attempt to lock h_high. The second lock() must
// throw std::logic_error (use EXPECT_THROW).
// Ensure h_low is released cleanly afterwards so subsequent tests are clean.
// ---------------------------------------------------------------------------
TEST(HierarchicalMutex, AscendingOrderThrows) {
  HierarchicalMutex high{10'000};
  HierarchicalMutex low{100};

  low.lock();
  EXPECT_THROW(high.lock(), std::logic_error);
  low.unlock();
}

// ---------------------------------------------------------------------------
// Test 3: Two threads acquiring in the same correct order — no deadlock,
// no throw, all iterations complete.
//
// Spin up 2 threads that each loop N times doing:
//     lock(h_high); lock(h_low); ...critical section...; unlock both.
// Join both. The test passes if both threads finish without exception.
// This is a "would deadlock under naive mutexes acquired in opposite
// orders across threads" check — here everyone respects the hierarchy.
// ---------------------------------------------------------------------------
TEST(HierarchicalMutex, TwoThreadsSameOrderNoDeadlock) {
  HierarchicalMutex high{10'000};
  HierarchicalMutex low{100};

  constexpr int    kIterations = 1'000;
  std::atomic<int> completed{0};

  auto lock_in_order = [&] {
    for (int i = 0; i < kIterations; ++i) {
      std::lock_guard<HierarchicalMutex> high_lock(high);
      std::lock_guard<HierarchicalMutex> low_lock(low);
      ++completed;
    }
  };

  std::thread first{lock_in_order};
  std::thread second{lock_in_order};
  first.join();
  second.join();

  // Both threads always acquire `high` before `low`. Neither ever holds
  // `low` while waiting for `high`, so an AB-BA deadlock is impossible.
  // The counter verifies neither thread was starved or aborted mid-loop.
  EXPECT_EQ(completed.load(), 2 * kIterations);
}

// ---------------------------------------------------------------------------
// Test 4: One thread violates, the other doesn't — violator throws,
// good thread proceeds.
//
// Thread A loops locking high→low correctly. Thread B loops locking
// low→high deliberately. Capture exceptions in each thread (e.g. via
// std::promise<std::exception_ptr> or an atomic counter) and assert:
//   - Thread A saw zero exceptions.
//   - Thread B saw std::logic_error every iteration.
// ---------------------------------------------------------------------------
TEST(HierarchicalMutex, PerThreadViolationIsIsolated) {
  HierarchicalMutex high{10'000};
  HierarchicalMutex low{100};

  constexpr int    kIterations = 500;
  std::atomic<int> good_completed{0};
  std::atomic<int> bad_violations{0};

  auto correct_order = [&] {
    for (int i = 0; i < kIterations; ++i) {
      std::lock_guard<HierarchicalMutex> hl(high);
      std::lock_guard<HierarchicalMutex> ll(low);
      ++good_completed;
    }
  };

  auto wrong_order = [&] {
    for (int i = 0; i < kIterations; ++i) {
      low.lock();
      try {
        high.lock();
        // Unreachable: high.lock() must throw because this thread's
        // hierarchy state was pushed down to 100 by low.lock() above.
        high.unlock();
      } catch (const std::logic_error&) {
        ++bad_violations;
      }
      low.unlock();
    }
  };

  std::thread good{correct_order};
  std::thread bad{wrong_order};
  good.join();
  bad.join();

  // The good thread completes every iteration despite contention with
  // the violator. The bad thread throws every iteration and is caught
  // locally — the exception never propagates into `good`. This is the
  // whole point of the `thread_local` hierarchy counter: each thread
  // enforces the discipline against its own state, no cross-thread cost.
  EXPECT_EQ(good_completed.load(), kIterations);
  EXPECT_EQ(bad_violations.load(), kIterations);
}

// ---------------------------------------------------------------------------
// Test 5: hierarchical_mutex composes with the RAII wrappers that unlock
// in LIFO order (lock_guard, unique_lock, single-mutex scoped_lock).
//
// It does NOT compose with multi-mutex std::scoped_lock — see the comment
// inside the test for why. This is a real teaching point, not a bug in
// our implementation: Williams' hierarchy check requires LIFO unlock, and
// libstdc++'s scoped_lock destructor unlocks in argument order.
// ---------------------------------------------------------------------------
TEST(HierarchicalMutex, ComposesWithRAIIWrappers) {
  HierarchicalMutex high{10'000};
  HierarchicalMutex low{100};

  // lock_guard (BasicLockable) — nested guards destruct in reverse order,
  // which matches the hierarchy's LIFO unlock requirement.
  {
    std::lock_guard<HierarchicalMutex> hg(high);
    std::lock_guard<HierarchicalMutex> lg(low);
  }

  // unique_lock (Lockable + state) — same LIFO destruction, plus explicit
  // unlock() available mid-scope.
  {
    std::unique_lock<HierarchicalMutex> uh(high);
    std::unique_lock<HierarchicalMutex> ul(low);
    ul.unlock();
    // uh still held; will unlock in dtor.
  }

  // Single-mutex scoped_lock works — one lock, one unlock, no ordering
  // question to get wrong.
  {
    std::scoped_lock lk(high);
    (void)lk;
  }

  // Deterministic hierarchy-violation check via nested unique_locks:
  // construction order = acquisition order, so we control it exactly.
  EXPECT_THROW(
      {
        std::unique_lock<HierarchicalMutex> ul_low(low);
        std::unique_lock<HierarchicalMutex> ul_high(high);
      },
      std::logic_error);

  // NOTE: multi-mutex std::scoped_lock(high, low) is intentionally NOT
  // tested here. Its destructor unlocks in argument order (high first),
  // but at that point this thread's hierarchy value is 100 (set by the
  // most recent lock of `low`), so our unlock() throws "unlock out of
  // order" — from a destructor during normal flow, causing std::terminate.
  // The mismatch is fundamental: HierarchicalMutex requires LIFO unlock,
  // multi-mutex scoped_lock does not guarantee it.
}
