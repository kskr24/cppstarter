# Chapter 3 — Sharing Data Between Threads (revision sheet)

Williams, *C++ Concurrency in Action* (1st ed).

---

## §3.1 — Core problem

- **Data race**: ≥2 threads access same location, ≥1 writes, no synchronization → **UB**.
- **Race condition** ≠ data race. Even fully thread-safe calls can race at the *interface* level (e.g. `top()` then `pop()`). Not UB, just wrong answers.
- Invariants are broken *transiently* mid-modification → why mutual exclusion matters.

---

## §3.1a — Data races: precise definition & detection

### The four-part test (C++ standard, [intro.races])

A **data race** exists iff **all** hold:

1. Two or more threads access the *same memory location*.
2. The accesses are *not ordered* by a happens-before relation.
3. At least one access is a *write*.
4. Neither access is a *lock-free atomic* op with sufficient ordering, and neither is inside a synchronizing primitive (mutex, `call_once`, thread start/join, etc.).

If any one fails, no data race. If all four hold, program has **UB** — not "wrong answer", not "torn read on x86", but the whole program is meaningless per the standard. Compiler is free to assume it never happens and optimize accordingly.

Key subtleties:

- **"Memory location"** ≠ variable. Two adjacent `bool`s or bit-fields packed into the same word are one location for race purposes on some ABIs. Since C++11, distinct scalar objects and distinct non-zero-width bit-fields *are* distinct memory locations by the standard — but tearing on adjacent sub-word writes is still a real hardware concern with `char`/`bool` fields.
- **Reads race with writes.** Two concurrent reads never race. One read + one write with no sync = race.
- **Same thread never races with itself** — sequenced-before is a happens-before.
- **`volatile` does NOT prevent data races.** It's for MMIO/signal handlers. It gives you neither atomicity nor ordering between threads.
- **`std::atomic<T>` with default `seq_cst`** is race-free by construction. Relaxed atomics avoid UB but may still allow surprising re-orderings (that's a *race condition*, not a data race).

### Sources of happens-before (what "synchronization" actually means)

Memorize this list — if none of these connects the two accesses, you have a race:

| Producer side                          | Consumer side                          |
|----------------------------------------|----------------------------------------|
| `mutex.unlock()`                       | subsequent `mutex.lock()` on same mtx  |
| `std::thread` ctor (launch)            | body of the new thread                 |
| end of thread                          | successful `join()`                    |
| `promise.set_value()`                  | `future.get()` / `wait()`              |
| `atomic` store (release)               | `atomic` load (acquire) that sees it   |
| `call_once` completion                 | later `call_once` on same flag         |
| `condition_variable.notify_*` + relock | `wait()` returning                     |
| destruction of `latch`/`barrier` phase | threads past the sync point            |

No entry from this table between two accesses to the same location + ≥1 write ⇒ race.

### How to spot one during code review (mechanical checklist)

Walk every shared variable and ask, in order:

1. **Who writes it?** List every write site.
2. **Who reads it?** List every read site.
3. **For each (writer, reader) or (writer, writer) pair on different threads: name the happens-before edge.** If you can't name one from the table above, it's a race.
4. **Is the "shared" variable actually shared?** Locals, thread_locals, and objects captured by-value into a `std::thread` are not.
5. **Const is not a shield.** `const` on a reference does not prevent someone else from writing through a non-const alias.
6. **Init counts.** Reading a global before its constructor finishes on another thread is a race (this is what `call_once` and function-local statics fix).
7. **Signed overflow / iterator invalidation / re-entrancy under lock** — separate bugs, don't confuse with races. But a mutating container touched by two threads with no lock is *always* a race regardless of the API.

### Smell list (things that *usually* mean a race is nearby)

- A `bool done;` / `int counter;` / raw pointer touched by more than one thread with no `atomic`, no mutex.
- A getter returning `T&` or `T*` to data protected only *inside* the class. The reference escapes the lock ⇒ race outside.
- `if (ptr) ptr->foo();` where `ptr` is written by another thread — classic TOCTOU + race.
- Lazy init via `if (!p) p = new T;` without `call_once`/atomic — DCLP, always UB (see §3.3.1).
- Cache/memoization field on an object shared across threads with only a `const` method interface — writer hides in `mutable`.
- Passing `this` (or captures like `[&]`) into a `std::thread` from within a ctor whose object still isn't fully constructed.
- Detached threads outliving referenced stack objects — race with destruction.
- Signal handlers touching non-`atomic`, non-`sig_atomic_t` state.

### Race condition (interface-level) vs data race

- **Data race** = the standard's definition above. Fix with sync primitives.
- **Race condition** = ordering-dependent bug that's still well-defined per thread. Example: `if (!stack.empty()) x = stack.top();` under a per-call mutex — each call is race-free, the sequence is not. Fix by widening the atomic unit of work (fuse operations, hold the lock longer, redesign the API — cf. Listing 3.5).
- Rule of thumb: if adding `std::atomic` or a bigger mutex around a *single* op fixes it, it was a data race. If you had to change the *API shape*, it was a race condition.

### Tools (use these; don't eyeball)

- **ThreadSanitizer** (`-fsanitize=thread`, clang/gcc): finds real happens-before violations at runtime. First line of defense. Cost ~5-15× slowdown; ship in CI.
- **Helgrind / DRD** (Valgrind): alternative; slower, sometimes more false positives, works without recompiling deps as aggressively.
- **UBSan** (`-fsanitize=undefined`): won't find races directly but catches the UB they often unmask.
- **`std::atomic_ref` (C++20)** — lets you audit a suspect plain variable by temporarily giving specific accesses atomic semantics without changing its type.
- Compile with `-Wthread-safety` (clang) if you annotate with capability attributes (`GUARDED_BY`, etc.) — static detection, no runtime cost.

TSan caveat: it only sees races on the paths executed. No coverage ⇒ no finding. Pair with stress tests / fuzzing.

### 60-second self-drill

Given code, in order:

1. Circle every shared mutable object.
2. For each, list all access sites and their threads.
3. Draw the happens-before edge for every cross-thread pair. Missing edge = race.
4. If the object is `atomic`, check that the memory order is strong enough for the invariant you want (not just "no UB").
5. If it's mutex-protected, check that *no reference/pointer escapes* the locked region.

---

## §3.2 — Mutex basics

- `std::mutex` — the primitive. Never `lock()`/`unlock()` by hand.
- RAII wrappers:
  - `std::lock_guard<M>(m)` — acquire in ctor, release in dtor. Non-movable.
  - `std::scoped_lock(m1, m2, …)` (**C++17**) — one or many, deadlock-safe.
- **Golden rule**: never let a pointer/reference to protected data escape the locked region (return refs, out params, user callbacks under lock).

---

## §3.2.3 — Thread-safe stack (Listing 3.5)

- `std::stack::pop()` returns `void` because a value-returning pop is exception-unsafe (Sutter GotW #8): if the return copy throws *after* mutation, value is lost.
- BUT: the STL split `top()` + `pop()` **creates** a new problem under concurrency — no lock spans both → interface race (double-consume, or top()-then-pop-wrong-element).
- Fix = **re-fuse** them into one locked op, without re-introducing Sutter's bug:
  - `std::shared_ptr<T> pop()` — allocate copy *before* mutating; alloc failure leaves stack intact.
  - `void pop(T& out)` — caller owns the sink; only copy-assign under lock.
- `mutable std::mutex m` — needed because `empty() const` must lock.
  - **Logical const** (observable state unchanged) vs **bitwise const** (no bit changes). `mutable` bridges them. That's exactly its purpose.
- Copy ctor: `lock(other.m)`, not `this->m`. `*this` isn't published yet; the *source* is the one another thread could mutate.

---

## §3.2.4 — Deadlock

- Cause: two threads acquire same mutexes in opposite order → **AB-BA** deadlock.
- Guidelines (memorize):
  1. Avoid nested locks.
  2. Avoid calling user code while holding a lock.
  3. Acquire multiple locks in a fixed global order.
  4. Use a lock hierarchy.

---

## §3.2.5 — Locking multiple mutexes safely

Four mechanisms, ascending modernness:

1. **Book idiom (C++11)**:
   ```cpp
   std::lock(m1, m2);                                  // atomic-ish, deadlock-safe
   std::lock_guard<std::mutex> la(m1, std::adopt_lock); // RAII, already-owned
   std::lock_guard<std::mutex> lb(m2, std::adopt_lock);
   ```
2. **`std::scoped_lock lk(m1, m2);`** (C++17) — one line, same semantics as above.
3. **Deferred `unique_lock`** — for when you need the locks to be movable/transferable:
   ```cpp
   std::unique_lock la(m1, std::defer_lock);
   std::unique_lock lb(m2, std::defer_lock);
   std::lock(la, lb);   // deadlock-safe acquire
   ```
4. **`hierarchical_mutex`** (Listing 3.8) — global lock ordering discipline. Only mechanism that scales across function/module boundaries. Uses `thread_local` hierarchy value; throws `std::logic_error` on out-of-order acquire.

Naive `lock_guard<M> a(m1); lock_guard<M> b(m2);` in a `swap(X,Y)` deadlocks trivially against `swap(Y,X)` on another thread.

---

## §3.2.6-7 — `std::unique_lock`

- Extra runtime state: `bool owns_lock`. Costs ~1 byte + branch in dtor. Negligible.
- Capabilities `lock_guard` lacks:
  1. **Movable** — return from factory, transfer between scopes.
  2. **Early `unlock()` / re-`lock()`** in the middle of a scope.
  3. **Deferred / try / timed** construction: `defer_lock`, `try_to_lock`, `try_lock_for/until`.
- Default preference: **`scoped_lock` > `lock_guard` > `unique_lock`.** Reach for `unique_lock` only when a feature above is needed.

---

## §3.2.8 — Granularity

- Lock as narrow a scope as correctness allows.
- Never hold a lock across slow/blocking operations (I/O, allocation of large objects, user callbacks).
- Fine-grained → more parallelism, more deadlock risk. Coarse-grained → simpler, less parallelism.

---

## §3.3 — Alternatives to plain mutex

### §3.3.1 — One-time initialization

- **DCLP is UB.** The outer null-check is unsynchronized w.r.t. the pointee's construction. Reader can see non-null pointer before pointee's ctor writes are visible.
- Fixes (best to worst for this use case):
  1. **Function-local `static`** — C++11 mandates thread-safe init. One line. Use when it fits.
  2. **`std::call_once(flag, fn)`** — exactly-once, establishes HB from init thread to every observer.
  3. **`std::atomic<std::shared_ptr<T>>`** (C++20) — real lock-free double-check with `acquire`/`release` ordering.

### §3.3.2 — Reader-writer locks

- 1st ed uses `boost::shared_mutex`. C++14: `std::shared_timed_mutex`. C++17: `std::shared_mutex`.
- Wrappers: readers use `std::shared_lock<M>`; writers use `std::unique_lock<M>`.
- Multiple readers OR one writer. Beware **writer starvation** under sustained read load.

### §3.3.3 — Recursive mutex

- Same thread may lock N times, must unlock N times.
- Usually a **design smell**: it means a helper is being called with the invariant already broken by the outer function. Prefer refactoring.

---

## Modern-C++ delta cheatsheet

| Book (C++11)                         | Modern replacement                        | Std      |
|--------------------------------------|-------------------------------------------|----------|
| `std::lock` + `adopt_lock` guards    | `std::scoped_lock`                        | C++17    |
| `boost::shared_mutex`                | `std::shared_mutex`                       | C++17    |
| DCLP with mutex                      | `call_once` / `atomic<shared_ptr<T>>`     | C++11/20 |
| Manual `defer_lock` + `std::lock`    | `std::scoped_lock` (if no transfer needed)| C++17    |
| Homegrown `scoped_thread`            | `std::jthread`                            | C++20    |

---

## Self-check (skim before you close the chapter)

1. Why does `std::stack::pop()` return `void`, and what problem does that split create for concurrent code?
2. AB-BA deadlock — sketch it in 30 seconds.
3. Three deadlock-safe ways to lock two mutexes, book-era → C++17.
4. `unique_lock` vs `lock_guard` — three real differences (no cheating with C++17).
5. Why `mutable std::mutex m`? State it in terms of **logical vs bitwise const**.
6. Why is DCLP UB *even with* the outer null-check?
7. What does `std::call_once` give you that a manual bool+mutex flag does not?

---

## One-liners worth memorizing

- Mutex protects **invariants**, not memory.
- If a pointer to protected data escapes, the mutex might as well not exist.
- `std::lock` = deadlock-safe **acquisition**; `lock_guard(…, adopt_lock)` = RAII **release**.
- `scoped_lock` = both, in one line, since C++17.
- `mutable` exists for **logical constness** — mutexes and caches are its poster children.
- DCLP without atomics/`call_once` is **always** UB. Not "usually", not "on x86". Always.
