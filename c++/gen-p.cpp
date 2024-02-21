// Compile with g++ gen-p.cpp -o run -lssl -lcrypto -pthread
#include <iostream>
#include <sstream>
#include <cstring>
#include <iomanip>

#include <vector>
#include <thread>
#include <mutex>
#include <openssl/sha.h>

#define TOTAL_NUM_HASHES 0x100000
#define HASH_LENGTH_TO_GENERATE 5

using namespace std;

string hashToHexString(unsigned char* hash) {
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


unsigned int hashToInt(unsigned char* hash) {
    unsigned int intHash = 0;
    for (char i = 0; (i*2) < HASH_LENGTH_TO_GENERATE; i++) {
        intHash = (intHash << 8) | hash[i];
    }
    if (HASH_LENGTH_TO_GENERATE % 2 == 1) {
        intHash >>= 4;
    }
    return intHash;
}

struct Job {
    string parentHash = "";
    int salt = 0;
    bool active = true;
};

class JobBoard {
    bool commitHasBeenGenerated[TOTAL_NUM_HASHES]{};
    unsigned int numCommits = 1;
    Job nextJob;
    mutex m;

    public:
    JobBoard(string initialCommitHash, unsigned int initialCommitIndex) {
        commitHasBeenGenerated[initialCommitIndex] = true;

        nextJob.parentHash = initialCommitHash;
        nextJob.active = true;
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

    void submitResult(Job& job, unsigned char* bitHash, int threadID) {
        m.lock();
        unsigned int hashIndex = hashToInt(bitHash);
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

void work(JobBoard* jobBoard, int threadID) {
    Job job = jobBoard->getJob();
    do {
        unsigned char* bitHash = genCommit(job.parentHash, job.salt);
        jobBoard->submitResult(job, bitHash, threadID);
        job = jobBoard->getJob();
    } while (job.active);
}

int main() {
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

    thread worker1(work, jobBoard, 1);
    // thread worker2(work, jobBoard, 2);
    // thread worker3(work, jobBoard, 3);
    // thread worker4(work, jobBoard, 4);

    worker1.join();
    // worker2.join();
    // worker3.join();
    // worker4.join();

    return 0;
}
