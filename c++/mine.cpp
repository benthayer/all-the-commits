// Example of how to use boost's lock free queue to synchronize threads
// Compile with ++ mine.cpp hashUtils.cpp -o run -lssl -lcrypto -lpthread
#include <cstring>
#include <thread>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <atomic>
#include <chrono>

#include <boost/lockfree/spsc_queue.hpp>
#include <openssl/rand.h>

#include "hashUtils.h"

using namespace std;
using namespace chrono;


using queue = boost::lockfree::spsc_queue<bool>;

void mine(int threadID, queue* myQueue, vector<queue*>* queues) {
  unsigned char initialSalt;
  RAND_bytes(&initialSalt, sizeof(initialSalt));
  string strHash = to_string(initialSalt);
  unsigned char* hash;
  unsigned int x;
  
  unsigned int target = 250;

  bool b;

  do {
    if (myQueue->pop(b)) {
      // cout << "Received break signal: " << threadID << endl;
      break;
    }
    hash = hashObject(strHash);
    x = hashToInt(hash, 7);
    strHash = hashToHexString(hash);
  } while (x > target); // Example condition, replace 10000 with desired threshold

  if (x <= target) {
    // cout << "Thread " << threadID << " mined " << x << endl;
    for (queue* theirQueue : *queues) {
      theirQueue->push(true);
    }
  }

  // cout << "Thread " << threadID << " stopped" << endl;
}

void setCoreAffinity(pthread_t pthread_handle, int core) {
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core, &cpuset);
  pthread_setaffinity_np(pthread_handle, sizeof(cpu_set_t), &cpuset);
}


void runThreads(int num_threads) {
  // cout << "Running with " << num_threads << " threads" << endl;

  // Vector to store thread objects
  vector<queue*> queues;
  vector<thread> threads;

  // Start threads
  for (int i = 0; i < num_threads; ++i) {
    // queue myQueue(10);
    queue* q = new queue(10);
    queues.push_back(q);
    threads.emplace_back(mine,i, queues[i], &queues);
    setCoreAffinity(threads.back().native_handle(), i % 8);
  }

  // Join threads
  for (auto& t : threads) {
    t.join();
  }
}

int test(int num_threads) {
  auto start_time = high_resolution_clock::now();
  runThreads(num_threads);
  auto end_time = high_resolution_clock::now();

  // Calculate the duration of the operation
  auto duration = duration_cast<nanoseconds>(end_time - start_time);
  cout << "Num_Threads: " << num_threads << " Execution time: " << duration.count()/1000000 << " milliseconds" << endl;
  return duration.count()/1000000;
}

void testMultiple(int num_threads) {
  long int average = 0;
  int attempts = 10;
  for (int i = 0; i < attempts; i++) {
    average += test(num_threads);
  }
  cout << "Average: " << average/attempts << endl;
}

int main() {
  testMultiple(1);
  testMultiple(2);
  testMultiple(4);
  testMultiple(8);
  testMultiple(16);
  return 0;
}


