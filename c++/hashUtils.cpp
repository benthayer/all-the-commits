// Compile with g++ gen-p.cpp -o run -lssl -lcrypto -pthread
#include <iostream>
#include <sstream>
#include <cstring>
#include <iomanip>

#include <cryptopp/sha.h>

using namespace std;
using namespace CryptoPP;


string hashToHexString(unsigned char* hash) {
    stringstream ss;
    for(int i=0; i<SHA1::DIGESTSIZE; ++i)
        ss << setw(2) << setfill('0') << hex << (int)hash[i];
    string hex_str = ss.str();
    return hex_str;
}


unsigned char* hashObject(const string& strObject) {
    unsigned char* hash = new unsigned char[SHA1::DIGESTSIZE];
    SHA1().CalculateDigest(hash, reinterpret_cast<const byte*>(strObject.data()), strObject.size());
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

