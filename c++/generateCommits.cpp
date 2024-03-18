// Compile with g++ singleThread.cpp hashUtils.cpp -o run -lssl -lcrypto
#include <cstring>
#include <iostream>
#include <iomanip>

#include "hashUtils.h"

using namespace std;

#include <chrono>

#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
using namespace CryptoPP;
using namespace chrono;

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
    unsigned int hashIndex = 0xc9f;
    generateCommits(startingHash, hashIndex, 3);

    return 0;
}
