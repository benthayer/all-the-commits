#include <iostream>
#include <chrono>
#include <botan-3/botan/hash.h>

// Function to calculate SHA-1 hash using Botan
void calculateSHA1(const std::string& message, std::vector<uint8_t>& hash) {
    Botan::SHA_160 sha1;
    sha1.update(reinterpret_cast<const uint8_t*>(message.data()), message.length());
    hash = sha1.final();
}

int main() {
    const int num_trials = 1000;
    const std::string message = "Hello, world!";
    std::vector<uint8_t> hash;

    // Compile command: g++ -o sha1_botan3 sha1_botan3.cpp $(pkg-config --libs --cflags botan-3)

    // Perform trials
    std::chrono::high_resolution_clock::time_point start, end;
    std::chrono::duration<double> total_time;

    for (int i = 0; i < num_trials; ++i) {
        start = std::chrono::high_resolution_clock::now();

        // Calculate SHA-1 hash
        calculateSHA1(message, hash);

        end = std::chrono::high_resolution_clock::now();
        total_time += end - start;
    }

    // Calculate average time per trial
    double average_time = total_time.count() / num_trials;

    std::cout << "Average time per trial: " << average_time << " seconds" << std::endl;

    return 0;
}

