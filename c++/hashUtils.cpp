// Compile with g++ gen-p.cpp -o run -lssl -lcrypto -pthread
#include <iostream>
#include <sstream>
#include <cstring>
#include <iomanip>

#include <openssl/sha.h>

using namespace std;

string hashToHexString(unsigned char* hash) {
    stringstream ss;
    for(int i=0; i<SHA_DIGEST_LENGTH; ++i)
        ss << setw(2) << setfill('0') << hex << (int)hash[i];
    string hex_str = ss.str();
    return hex_str;
}

#include <chrono>


unsigned char* hashObject(string& strObject) {
    auto start = chrono::high_resolution_clock::now();
    unsigned char* hash = (unsigned char*)malloc(sizeof(char) * SHA_DIGEST_LENGTH); // == 20
    unsigned char object[strObject.length()];
    for (int i = 0; i < strObject.length(); i++) {
        object[i] = (unsigned char)strObject[i];
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Time taken by function: " << duration.count() << " nanoseconds" << std::endl;
    start = chrono::high_resolution_clock::now();
    SHA1(object, sizeof(object), hash);
    end = chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Time taken by function: " << duration.count() << " nanoseconds" << std::endl;
    return hash;

}

unsigned char* genCommit(string& parentHash, int salt) {
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


unsigned int hashToInt(unsigned char* hash, int length) {
    unsigned int intHash = 0;
    for (char i = 0; (i*2) < length; i++) {
        intHash = (intHash << 8) | hash[i];
    }
    if (length % 2 == 1) {
        intHash >>= 4;
    }
    return intHash;
}

