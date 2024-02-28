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

#include <sched.h>
#include <boost/lockfree/spsc_queue.hpp>
#include <openssl/rand.h>

#include "hashUtils.h"


void worker(jobQueue, solutionQueue) {
  bool awaitedJob = false;
  while (true) {
    if (awaitedJob || jobQueue.pop()) {
      // update parent hash and stuff
      awaitedJob = false;
    }
    // do sha1
    if (isSolution()) {
      solutionQueue.push();
      while (!jobQueue.pop()) {}
      awaitedJob = true;
    }
  }
}

void controller(jobQueues, solutionQueues) {
  for (auto& solutionQueue : solutionQueues) {
    if (solutionQueue.pop()) {
      if (validateSolution(solution)) {
        // update current job
        for (auto& jobQueue: jobQueues) {
          jobQueue.push();
        }
      }
    }
  }
}

// Create threads & queues from the beginning
// In the future, use controller to manage queue creation
// Controller can create workers at will
// With each job, worker are informed of the current modulus. Their thread id is less than the modu

int main() {
  // generate queues
  // 
  return 0;
}