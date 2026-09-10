#pragma once

#include <climits>
#include <limits>
#include <mutex>
#include <stdexcept>

class HierarchicalMutex {
  std::mutex          internal_mutex;
  unsigned long const hierarchical_value;
  unsigned long       previous_hierarchy_value;

  inline static thread_local unsigned long this_thread_hierarchy_value =
      std::numeric_limits<unsigned long>::max();

  void check_for_hierarchy_violation() const {
    if (this_thread_hierarchy_value <= hierarchical_value) {
      throw std::logic_error("mutex hierarchy violated");
    }
  }

  void update_hierarchy_value() noexcept {
    previous_hierarchy_value    = this_thread_hierarchy_value;
    this_thread_hierarchy_value = hierarchical_value;
  }

public:
  explicit HierarchicalMutex(unsigned long value) noexcept
      : hierarchical_value(value), previous_hierarchy_value(0) {}

  HierarchicalMutex(const HierarchicalMutex&)            = delete;
  HierarchicalMutex& operator=(const HierarchicalMutex&) = delete;

  void lock() {
    check_for_hierarchy_violation();
    internal_mutex.lock();
    update_hierarchy_value();
  }

  void unlock() {
    if (this_thread_hierarchy_value != hierarchical_value) {
      throw std::logic_error("unlock out of order");
    }
    this_thread_hierarchy_value = previous_hierarchy_value;
    internal_mutex.unlock();
  }

  [[nodiscard]] bool try_lock() {
    check_for_hierarchy_violation();
    if (!internal_mutex.try_lock()) {
      return false;
    }
    update_hierarchy_value();
    return true;
  }
};
