// Compile with g++ singleThread.cpp hashUtils.cpp -o run -lssl -lcrypto
#include <cstring>
#include <iostream>

#include "hashUtils.h"

using namespace std;

void generateCommits(string parentHash, unsigned int parentHashIndex, int length) {
    unsigned int TOTAL_NUM_HASHES = 1 << (length * 4);

    bool* generated = new bool[TOTAL_NUM_HASHES];
    generated[parentHashIndex] = true;

    unsigned int numCommits = 1; // Parent hash is the first
    int salt = 0;
    while (numCommits != TOTAL_NUM_HASHES) {
        unsigned char* bithash = genCommit(parentHash, salt);
        string hash = hashToHexString(bithash);
        unsigned int hashIndex = hashToInt(bithash, length);
        if (!generated[hashIndex]) {
            // Log update
            generated[hashIndex] = true;
            // cout << hash << "," << salt << endl;

            // Update variables
            parentHash = hash;
            numCommits++;
            salt = 1;

            if (((numCommits % 10000) == 0) || (numCommits > (TOTAL_NUM_HASHES - 10000))) {
                // Track progress
                cout << "\r" << numCommits << flush;
            }
        } else {
            salt++;
        }
        delete[] bithash;
    }
}

int main() {
    string startingHash = "c9fc3d97717367b5fa45709a70a15ddd657f0275";
    // unsigned int hashIndex = 0xc9fc3d9;
    unsigned int hashIndex = 0xc9fc3;
    generateCommits(startingHash, hashIndex, 5);

    return 0;
}
