#include <iostream>
#include <iomanip>
#include <chrono>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

using namespace std;
using namespace CryptoPP;

int main() {
    const int num_trials = 1000;
    const string message = "Hello, world!";

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < num_trials; ++i) {
        SHA1 sha1;
        byte digest[SHA1::DIGESTSIZE];
        sha1.CalculateDigest(digest, (const byte*)message.data(), message.size());
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

    cout << "Time taken for " << num_trials << " trials: " << duration << " microseconds" << endl;

    return 0;
}

