#include <thread>
#include <mutex>
#include <openssl/sha.h>

namespace benthayer;


struct HashInputs {
    std::string parent;
    unsigned int salt = 0;
}

class Box {
    HashInputs hashInputs;
    bool commits[];
    vector<HashInputs> write_queue;
    mutex m;

    HashInputs getJob() {
        m.Lock();
        hashInputs.salt++;
        HashInputs nextJob(self.hashInputs.parent, self.hashInputs.salt)
        m.Unlock();
        return nextJob;
    }

    HashInputs getNextJob(HashInputs inputs, unsigned char[] hash) {
        m.Lock();

        if !commits[hash] {
            this.hashInputs.parent = hash;
            this.hashInputs.salt = 1;
            this.commits[hash] = true;
            this.write_queue.push(inputs);
        } else {
            this.salt++;
        }

        HashInputs nextJob(self.hashInputs.parent, self.hashInputs.salt)

        m.Unlock();
        return nextJob;
    }
}

void writer() {
    something
}

unsigned char[] getHash(HashInputs inputs) {
    std::string tree = "tree 6a0165c2aea6cfc5fba01029ede7a8da6c85f6f6";
    std::string parent = "parent " + inputs.parent;
    std::string author = "author Ben Thayer <ben@benthayer.com> 1599715620 -0500";
    std::string committer = "committer Ben Thayer <ben@benthayer.com> 1599715620 -0500";
    std::string message = "Initial commit";

    std::string commit = tree + "\n" +
        parent + "\n" +
        author + "\n" +
        committer + "\n\n" +
        message + "\n\n" +
        salt + "\n";

    std::string object = "commit " + commit.length() + "\x00";

    unsigned char hash[SHA_DIGEST_LENGTH]; // == 20

    SHA1(object, sizeof(str) - 1, hash);
    return hash
}

void work() {
    HashInputs job = box.getJob();
    while job != NULL {
        unsigned char[] hash = getHash(job);
        job = box.getNextJob(job, hash);
    }
}

void spawnWorker() {
    thread t();
}

void main() {
    for int i = 0; i < 8; i++ {
        spawnWorker();
    }
    thread t(writer);
}
