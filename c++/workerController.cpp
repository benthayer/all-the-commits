

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

int main() {

  return 0;
}