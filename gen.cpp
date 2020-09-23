// Compile with g++ paralell-base.cpp -o run -lssl -lcrypto
#include <iostream>
#include <sstream>
#include <cstring>

#include <vector>
#include <thread>
#include <mutex>
#include <openssl/sha.h>

using namespace std;

string hexsha(unsigned char hash[SHA_DIGEST_LENGTH]) {
    stringstream ss;
    for(int i=0; i<SHA_DIGEST_LENGTH; ++i)
        ss << hex << (int)hash[i];
    string hex_str = ss.str();
    return hex_str;
}


string hashObject(string strObject) {
    unsigned char hash[SHA_DIGEST_LENGTH]; // == 20
    unsigned char object[strObject.length()];
    for (int i = 0; i < strObject.length(); i++) {
        object[i] = (unsigned char)strObject[i];
    }
    SHA1(object, sizeof(object), hash);

    return hexsha(hash);

}

string genCommit(string parentHash, int salt) {
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

int main() {
    // test to make sure things work
    string testParentHash = "c9fc3d97717367b5fa45709a70a15ddd657f0275";
    string testHash = "2e835cf071ae2a3eda7482cca7a0177e79d1f047";
    if (genCommit(testParentHash, 0).compare(testHash)) {
        cout << "Test commit has wrong hash! Exiting" << endl;
        return 1;
    }




    return 0;
}
