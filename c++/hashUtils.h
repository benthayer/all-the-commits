
#include <cstring>

using namespace std;

string hashToHexString(unsigned char* hash);
unsigned char* hashObject(string& strObject);
unsigned char* genCommit(string& parentHash, int salt);
unsigned int hashToInt(unsigned char* hash, int length);
