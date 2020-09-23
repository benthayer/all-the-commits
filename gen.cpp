// Compile with g++ paralell-base.cpp -o run -lssl -lcrypto
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

int main() {
    // test to make sure things work
    string testParentHash = "c9fc3d97717367b5fa45709a70a15ddd657f0275";
    string generatedHash = hexsha(genCommit(testParentHash, 0));
    string testHash = "2e835cf071ae2a3eda7482cca7a0177e79d1f047";
    cout << generatedHash << endl;
    cout << testHash << endl;
    if (generatedHash.compare(testHash)) {
        cout << "Test commit has wrong hash! Exiting" << endl;
        return 1;
    }

    bool* commitHasBeenGenerated = new bool[TOTAL_NUM_HASHES];
    unsigned int numCommits = 1;
    commitHasBeenGenerated[211796953] = true;

    // test to make sure this isn't out of range
    unsigned char maxIntHash[4] = {255, 255, 255, 255};
    cout << hashAsIndex(maxIntHash) << endl;
    commitHasBeenGenerated[hashAsIndex(maxIntHash)];

    string parentHash = testParentHash;
    unsigned char* bithash;
    bithash= new unsigned char[20] {201, 252, 61, 144, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    cout << "Hash as index" << endl;
    cout << testParentHash << endl;
    cout << hexsha(bithash) << endl;
    cout << hashAsIndex(bithash) << endl;
    string hash;
    while (numCommits != TOTAL_NUM_HASHES) {
        int salt = 0;
        while (commitHasBeenGenerated[hashAsIndex(bithash)]) {
            bithash = genCommit(parentHash, salt);
            hash = hexsha(bithash);
            salt++;
        }
        cout << parentHash << " " << to_string(salt) << endl;
        parentHash = hash;
        commitHasBeenGenerated[hashAsIndex(bithash)] = true;
        numCommits++;
    }

    return 0;
}
