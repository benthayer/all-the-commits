#include <iostream>
#include <fstream>

void binaryDemo() {
    // Example SHA-1 hash value stored as unsigned char*
    unsigned char sha1Hash[20] = {0x2e, 0x24, 0x56, 0x87, 0x3a, 0xbf, 0xc9, 0x0e, 0x1d, 0xf5, 
                                  0x4a, 0x02, 0x78, 0x96, 0x3c, 0x5d, 0x78, 0x91, 0x62, 0x45};

    // Open a file for writing
    std::ofstream outputFile("output.bin", std::ios::binary);

    // Check if the file is opened successfully
    if (outputFile.is_open()) {
        // Write the SHA-1 hash to the file
        outputFile.write(reinterpret_cast<const char*>(sha1Hash), sizeof(sha1Hash));

        // Close the file
        outputFile.close();
        std::cout << "SHA-1 hash has been written to the file successfully." << std::endl;
    } else {
        std::cerr << "Error: Unable to open the file for writing." << std::endl;
    }
}

void textDemo() {
    // Open a file for writing
    std::ofstream outputFile("output.txt");

    // Check if the file is opened successfully
    if (outputFile.is_open()) {
        // Write to the file
        outputFile << "Hello, World!" << std::endl;
        outputFile << "This is a line written to the file.";

        // Close the file
        outputFile.close();
        std::cout << "Data has been written to the file successfully." << std::endl;
    } else {
        std::cerr << "Error: Unable to open the file for writing." << std::endl;
    }
}

int main() {
  binaryDemo();
  textDemo();
}