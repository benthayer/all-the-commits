#include <iostream>
#include <chrono>

// Crypto++
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

// Botan
#include <botan/hash.h>

// OpenSSL
#include <openssl/sha.h>

// Boost.Crypto
#include <boost/crypto/hash.hpp>

// libsodium
#include <sodium.h>

void runCryptoPPSha1Iterations() {
    using namespace CryptoPP;
    using namespace std::chrono;

    SHA1 sha1;
    std::string input = "input_data"; // Your input data here
    std::string output;

    for (int i = 0; i < 10; ++i) {
        auto start = high_resolution_clock::now();
        StringSource(input, true, new HashFilter(sha1, new HexEncoder(new StringSink(output))));
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start);
        std::cout << "Crypto++ - Run " << i+1 << ": " << duration.count() << " nanoseconds" << std::endl;
    }
}

void runBotanSha1Iterations() {
    using namespace Botan;
    using namespace std::chrono;

    auto hash_func = HashFunction::create("SHA-1");
    std::string input = "input_data"; // Your input data here

    for (int i = 0; i < 10; ++i) {
        auto start = high_resolution_clock::now();
        hash_func->update(reinterpret_cast<const uint8_t*>(input.data()), input.size());
        hash_func->final();
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start);
        std::cout << "Botan - Run " << i+1 << ": " << duration.count() << " nanoseconds" << std::endl;
    }
}

void runOpenSSLSha1Iterations() {
    using namespace std::chrono;

    std::string input = "input_data"; // Your input data here

    for (int i = 0; i < 10; ++i) {
        SHA_CTX ctx;
        SHA1_Init(&ctx);
        auto start = high_resolution_clock::now();
        SHA1_Update(&ctx, input.c_str(), input.length());
        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1_Final(hash, &ctx);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start);
        std::cout << "OpenSSL - Run " << i+1 << ": " << duration.count() << " nanoseconds" << std::endl;
    }
}

void runBoostCryptoSha1Iterations() {
    using namespace boost::crypto;
    using namespace std::chrono;

    std::string input = "input_data"; // Your input data here

    for (int i = 0; i < 10; ++i) {
        auto start = high_resolution_clock::now();
        auto hash = hash_value("SHA-1", input);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start);
        std::cout << "Boost.Crypto - Run " << i+1 << ": " << duration.count() << " nanoseconds" << std::endl;
    }
}

void runLibsodiumSha1Iterations() {
    using namespace std::chrono;

    std::string input = "input_data"; // Your input data here

    for (int i = 0; i < 10; ++i) {
        auto start = high_resolution_clock::now();
        unsigned char hash[crypto_hash_sha1_BYTES];
        crypto_hash_sha1(hash, reinterpret_cast<const unsigned char*>(input.c_str()), input.length());
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start);
        std::cout << "libsodium - Run " << i+1 << ": " << duration.count() << " nanoseconds" << std::endl;
    }
}

int main() {
    // Initialize libsodium
    if (sodium_init() == -1) {
        std::cerr << "Error initializing libsodium\n";
        return 1;
    }

    // Run iterations for each library
    runCryptoPPSha1Iterations();
    runBotanSha1Iterations();
    runOpenSSLSha1Iterations();
    runBoostCryptoSha1Iterations();
    runLibsodiumSha1Iterations();

    return 0;
}
