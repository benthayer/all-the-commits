#include <cstring>
#include <thread>
#include <vector>
#include <iostream>
#include <atomic>

#include "hashUtils.h"

using namespace std;

atomic<bool> stop_flag(false); // Atomic flag to signal threads to stop

void mine(int threadID) {
  string strHash = "";
  unsigned char* hash;
  unsigned int x;
  
  unsigned int target = 75;

  do {
    hash = hashObject(strHash);
    x = hashToInt(hash, 7);
    strHash = hashToHexString(hash);
  } while (x > target && !stop_flag); // Example condition, replace 10000 with desired threshold

  if (x <= target) {
    cout << "Thread " << threadID << " mined " << x << endl;
  }

  // Trigger all threads to stop
  stop_flag = true;

  cout << "Thread " << threadID << " stopped" << endl;
}


int main() {
  // Define number of threads
  const int num_threads = 1; // Example: 4 threads

  cout << "Running with " << num_threads << " threads" << endl;

  // Vector to store thread objects
  vector<thread> threads;

  // Start threads
  for (int i = 0; i < num_threads; ++i) {
    threads.push_back(thread(mine, i));
  }

  // Join threads
  for (auto& t : threads) {
    t.join();
  }

  return 0;
}
