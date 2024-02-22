// Compile with g++ gen-p.cpp -o run -lssl -lcrypto -pthread
#include <iostream>
#include <sstream>
#include <cstring>

#include <thread>
#include <mutex>

#include "hashUtils.h"

#define TOTAL_NUM_HASHES 0x100000
#define HASH_LENGTH_TO_GENERATE 5

using namespace std;

struct Job {
    string parentHash = "";
    int salt = 0;
    bool active = true;
};


class JobBoard {
    bool commitHasBeenGenerated[TOTAL_NUM_HASHES]{};
    unsigned int numCommits = 1;
    Job nextJob;
    thread workers[8];
    int numWorkers = 0;
    int maxWorkers = 8;
    mutex m;

    void work() {
        Job job = this->getJob();
        do {
            unsigned char* bitHash = genCommit(job.parentHash, job.salt);
            if (this->checkResult(job, bitHash)) {
                this->submitResult(job, bitHash);
            }
            job = this->getJob();
        } while (job.active);
    }

    public:
    JobBoard(string initialCommitHash, unsigned int initialCommitIndex) {
        commitHasBeenGenerated[initialCommitIndex] = true;

        nextJob.parentHash = initialCommitHash;
        nextJob.active = true;
    }

    void startWorker() {
        if (numWorkers < maxWorkers) {
            workers[numWorkers++] = thread(&JobBoard::work, this);
        }
    }

    void joinWorkers() {
        for (int i = 0; i < 8; i++) {
            if (workers[i].joinable()) {
                workers[i].join();
            }
        }
    }

    Job getJob(){
        m.lock();
        if (numCommits == TOTAL_NUM_HASHES) {
            nextJob.active = false;
        }
        Job job = nextJob;
        nextJob.salt++;
        // cout << "JOB," << job.parentHash << "," << to_string(job.salt) << endl;
        m.unlock();
        return job;
    }

    bool checkResult(Job&job, unsigned char* bitHash) {
        unsigned int hashIndex = hashToInt(bitHash, HASH_LENGTH_TO_GENERATE);
        return !commitHasBeenGenerated[hashIndex];
    }

    void submitResult(Job& job, unsigned char* bitHash) {
        m.lock();
        unsigned int hashIndex = hashToInt(bitHash, HASH_LENGTH_TO_GENERATE);
        if (!commitHasBeenGenerated[hashIndex]) {
            // Update progress
            commitHasBeenGenerated[hashIndex] = true;
            numCommits++;

            // Save commit
            string hexHash = hashToHexString(bitHash);
            // cout << "SLN," << job.parentHash << "," << to_string(job.salt) << "," << hexHash << "," << numCommits<< endl;
            // Set up next commit
            nextJob.parentHash = hexHash;
            nextJob.salt = 1;
            cout << "\r" << numCommits << flush;
        }
        delete[] bitHash;
        m.unlock();
    }
};


int generateCommits() {
    if (TOTAL_NUM_HASHES != (1 << (HASH_LENGTH_TO_GENERATE * 4))) {
        cout << "Inputs incorreect" << endl;
        return 1;
    }

    string initialCommitHash = "c9fc3d97717367b5fa45709a70a15ddd657f0275";
    // unsigned int initialCommitIndex = 0xc9fc3d9;
    unsigned int initialCommitIndex = 0xc9f;

    if (initialCommitIndex >= TOTAL_NUM_HASHES) {
        cout << initialCommitIndex << endl;
        cout << TOTAL_NUM_HASHES << endl;
        cout << "initial Commit Index too big" << endl;
        return 1;
    }

    JobBoard* jobBoard = new JobBoard(initialCommitHash, initialCommitIndex);

    jobBoard->startWorker();
    jobBoard->joinWorkers();
    return 0;
}


int main() {
    return generateCommits();
}
