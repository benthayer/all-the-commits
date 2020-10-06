// Compile with g++ gen-p.cpp -o run -lssl -lcrypto -pthread
#include <iostream>
#include <sstream>
#include <cstring>
#include <iomanip>

#include <vector>
#include <thread>
#include <mutex>
#include <openssl/sha.h>

#define TOTAL_NUM_HASHES 268435456

using namespace std;

string hexsha(unsigned char* hash) {
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
    mutex m;

    public:
    JobBoard() {
        commitHasBeenGenerated[211796953] = true;
        nextJob.active = true;
    }

    Job getJob(){
        m.lock();
        if (numCommits == TOTAL_NUM_HASHES) {
            nextJob.active = false;
        }
        Job job = nextJob;
        nextJob.salt++;
        cout << job.hash << " " << to_string(job.salt) << " DISPATCH" << endl;
        m.unlock();
        return job;
    }

    void submitResult(Job& job, unsigned char* bitHash, int tId) {
        m.lock();
        unsigned int hashIndex = hashAsIndex(bitHash);
        if (!commitHasBeenGenerated[hashIndex]) {
            // Save commit
            cout << job.hash << " " << to_string(job.salt) << " " << hexsha(bitHash) << endl;
            commitHasBeenGenerated[hashIndex] = true;
            numCommits++;

            // Set up next commit
            nextJob.hash = hexsha(bitHash);
            nextJob.salt = 1;
        }
        delete[] bitHash;
        m.unlock();
    }
};

void work(JobBoard* jobBoard, int tId) {
    Job job = jobBoard->getJob();
    do {
        unsigned char* bitHash = genCommit(job.hash, job.salt);
        jobBoard->submitResult(job, bitHash, tId);
        job = jobBoard->getJob();
    } while (job.active);
}

int main() {
    JobBoard* jobBoard = new JobBoard();
    thread t(work, jobBoard, 1);
    thread t2(work, jobBoard, 2);

    t.join();
    t2.join();

    return 0;
}
