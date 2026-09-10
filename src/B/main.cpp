#include <bits/stdc++.h>
#include <unistd.h>
#include <mutex>
#include <thread>

using namespace std;

class ThreadGuard {
public:
  explicit ThreadGuard(std::thread& t) : t_(t) {}
  
  ~ThreadGuard() {
    if (t_.joinable()) {
      t_.join();
    }
  }

private:
  std::thread& t_;
};

void solve() { std::cout << "From the new thread\n"; }

int main() {
  // int t;
  // cin >> t;
  // while (t--) {
  //   solve();
  // }
  std::cout << "Starting the main thread\n";
  std::thread t(solve);
  ThreadGuard guard(t);
  sleep(5);
  std::cout << "Exiting the main thread\n";
  return 0;
}