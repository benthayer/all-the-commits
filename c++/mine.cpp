// Example of how to use boost's lock free queue to synchronize threads
// Compile with ++ mine.cpp hashUtils.cpp -o run -lssl -lcrypto -lpthread
#include <cstring>
#include <thread>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <atomic>

#include <boost/lockfree/spsc_queue.hpp>

#include "hashUtils.h"

using namespace std;

using queue = boost::lockfree::spsc_queue<bool>;

void mine(int threadID, queue* myQueue, vector<queue*>* queues) {
  string strHash = to_string(rand());
  unsigned char* hash;
  unsigned int x;
  
  unsigned int target = 80;

  bool b;

  do {
    if (myQueue->pop(b)) {
      cout << "Received break signal: " << threadID << endl;
      break;
    }
    hash = hashObject(strHash);
    x = hashToInt(hash, 7);
    strHash = hashToHexString(hash);
  } while (x > target); // Example condition, replace 10000 with desired threshold

  if (x <= target) {
    cout << "Thread " << threadID << " mined " << x << endl;
    for (queue* theirQueue : *queues) {
      theirQueue->push(true);
    }
  }

  cout << "Thread " << threadID << " stopped" << endl;
}


int main() {
  srand(time(0));

  // Define number of threads
  const int num_threads = 2; // Example: 4 threads

  cout << "Running with " << num_threads << " threads" << endl;

  // Vector to store thread objects
  vector<queue*> queues;
  vector<thread> threads;

  // Start threads
  for (int i = 0; i < num_threads; ++i) {
    // queue myQueue(10);
    queue* q = new queue(10);
    queues.push_back(q);
    threads.emplace_back(mine,i, queues[i], &queues);
  }

  // Join threads
  for (auto& t : threads) {
    t.join();
  }

  return 0;
}
