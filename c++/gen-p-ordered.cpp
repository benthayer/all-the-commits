// Compile with g++ gen-p.cpp -o run -lssl -lcrypto -pthread
#include <iostream>
#include <sstream>
#include <cstring>
#include <iomanip>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <openssl/sha.h>

#define TOTAL_NUM_HASHES 268435456

using namespace std;

string getHexSHA(unsigned char* hash) {
    stringstream ss;
    for(int i=0; i<SHA_DIGEST_LENGTH; ++i)
        ss << setw(2) << setfill('0') << hex << (int)hash[i];
    string hex_str = ss.str();
    return hex_str;
}


unsigned char* hashObject(string strObject) {
    unsigned char* hash = (unsigned char*)malloc(sizeof(char) * SHA_DIGEST_LENGTH); // == 20
    unsigned char object[strObject.length()];
    for (int i = 0; i < strObject.length(); i++) {
        object[i] = (unsigned char)strObject[i];
    }
    SHA1(object, sizeof(object), hash);

    return hash;

}

unsigned char* genCommit(string parentHash, int salt) {
    string tree("tree f9baff06993b951e53c0312442241c6ee30921a6\n");
    string content(
        "author Ben Thayer <ben@benthayer.com> 1600063486 -0500\n"
        "committer Ben Thayer <ben@benthayer.com> 1600063486 -0500\n"
        "\n"
        "Added gen_commits to .gitignore\n\n");

    string parent("parent " + parentHash + "\n");

    string commit;
    commit.append(tree);
    commit.append(parent);
    commit.append(content);
    commit.append(to_string(salt) + "\n");

    string object("commit ");
    object.append(to_string(commit.length()));
    object.push_back('\0');
    object.append(commit);

    return hashObject(object);
}


unsigned int hashAsIndex(unsigned char* hash) {
    unsigned int intHash = int(
            hash[0] << 24 |
            hash[1] << 16 |
            hash[2] << 8 |
            hash[3]
            );
    return intHash >> 4;
}

struct Job {
    string hash = "c9fc3d97717367b5fa45709a70a15ddd657f0275";
    int salt = 0;
    bool active = true;
};

class JobBoard {
    bool commitHasBeenGenerated[TOTAL_NUM_HASHES]{};
    unsigned int numCommits = 1;

    Job nextJob;
    mutex jobMutex;

    bool processingSolution = false;
    mutex nominationMutex;
    Job bestSolution;
    unsigned char* bestSHA;
    mutex solutionProcessingMutex;
    condition_variable solutionReady;
    condition_variable solutionProcessed;

    public:
    JobBoard() {
        commitHasBeenGenerated[211796953] = true;
        nextJob.active = true;
    }

    Job getJob(){
        jobMutex.lock();
        if (numCommits == TOTAL_NUM_HASHES) {
            nextJob.active = false;
        }
        Job job = nextJob;
        nextJob.salt++;
        cout << job.hash << " " << to_string(job.salt) << " DISPATCH" << endl;
        jobMutex.unlock();
        return job;
    }

    void nominateSolution(Job& candidate, unsigned char* hash) {
        nominationMutex.lock();
        processingSolution = true;
        if (!processingSolution) {
            bestSolution = candidate;
        } else if (candidate.salt < bestSolution.salt) {
            bestSolution.salt = candidate.salt;
            bestSHA = hash;
        }
        solutionReady.notify_one();
        nominationMutex.unlock();
    }

    void submitResult(Job& job, unsigned char* bitHash, int tId) {
        m.lock();
        unsigned int hashIndex = hashAsIndex(bitHash);
        if (!commitHasBeenGenerated[hashIndex]) {
            string hexSHA = getHexSHA(bitHash);
            nominateSolution(job, hexSHA);
        }
        delete[] bitHash;
        m.unlock();
    }
};

void work(JobBoard* jobBoard, int tId) {
    mutex m;
    unique_lock<mutex> workLock(m);
    Job job;
    while {
        if (jobBoard.processingSolution) {
            jobBoard.solutionProcessed.wait(workLock);
        }
        job = jobBoard->getJob();
        if (!job.active)
            break
        unsigned char* bitHash = genCommit(job.hash, job.salt);
        jobBoard->submitResult(job, bitHash, tId);
    }
}

int main() {
    int numThreads = thread::hardware_concurrency();
    JobBoard* jobBoard = new JobBoard();
    unique_lock<mutex> solutionProcessingLock(jobBoard.solutionProcessingMutex);

    thread t(work, jobBoard, 1);
    thread t2(work, jobBoard, 2);

    while (numCommits < TOTAL_NUM_HASHES) {
        jobBoard.solutionReady.wait(solutionProcessingLock);
        jobBoard.bestSolution;
        // Save commit
        commitHasBeenGenerated[hashIndex] = true;
        numCommits++;

        // Set up next commit
        nextJob.hash = getHexSHA(bitHash);
        nextJob.salt = 1;
        cout << jobBoard.bestSolution.hash << " " <<
            to_string(jobBoard.bestSolution.salt) << " " <<
            jobBoard.bestHex << endl;
        jobBoard.processingSolution = false;
        jobBoard.solutionProcessed.notify_all();
    }

    t.join();
    t2.join();

    return 0;
}
