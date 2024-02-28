#include <iostream>
#include <chrono>
#include <openssl/sha.h>

using namespace std;
using namespace std::chrono;

int main() {
    const string input = "Hello, world!"; // Sample input for hashing
    const int num_trials = 1000;

    auto start = high_resolution_clock::now();

    for (int i = 0; i < num_trials; ++i) {
        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1(reinterpret_cast<const unsigned char*>(input.c_str()), input.length(), hash);
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - start);

    cout << "Time taken for " << num_trials << " trials of SHA-1 using OpenSSL: "
         << duration.count() << " nanoseconds" << endl;

    return 0;
}

