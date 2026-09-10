#include "threadsafe_stack.hpp"

#include <gtest/gtest.h>

#include <string>
#include <thread>
#include <vector>

TEST(ThreadsafeStack, PopByRefReturnsLIFOOrder) {
  ThreadsafeStack<int> s;
  s.push(1);
  s.push(2);
  s.push(3);

  int v = 0;
  s.pop(v);
  EXPECT_EQ(v, 3);
  s.pop(v);
  EXPECT_EQ(v, 2);
  s.pop(v);
  EXPECT_EQ(v, 1);
  EXPECT_TRUE(s.empty());
}

TEST(ThreadsafeStack, PopReturnsSharedPtrWithValue) {
  ThreadsafeStack<std::string> s;
  s.push("hello");

  auto p = s.pop();
  ASSERT_NE(p, nullptr);
  EXPECT_EQ(*p, "hello");
  EXPECT_TRUE(s.empty());
}

TEST(ThreadsafeStack, PopOnEmptyThrows) {
  ThreadsafeStack<int> s;
  EXPECT_THROW(s.pop(), empty_stack);

  int v = 0;
  EXPECT_THROW(s.pop(v), empty_stack);
}

TEST(ThreadsafeStack, EmptyReflectsState) {
  ThreadsafeStack<int> s;
  EXPECT_TRUE(s.empty());
  s.push(42);
  EXPECT_FALSE(s.empty());
}

TEST(ThreadsafeStack, ConcurrentPushesPreserveCount) {
  ThreadsafeStack<int> s;
  constexpr int        kThreads   = 8;
  constexpr int        kPerThread = 1000;

  std::vector<std::thread> workers;
  workers.reserve(kThreads);
  for (int t = 0; t < kThreads; ++t) {
    workers.emplace_back([&s, t] {
      for (int i = 0; i < kPerThread; ++i) {
        s.push(t * kPerThread + i);
      }
    });
  }
  for (auto& w : workers)
    w.join();

  // All producers joined, so single-reader pop-until-empty is race-free here.
  int popped = 0;
  int v      = 0;
  while (!s.empty()) {
    s.pop(v);
    ++popped;
  }
  EXPECT_EQ(popped, kThreads * kPerThread);
}
