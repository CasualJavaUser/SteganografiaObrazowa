#include <filesystem>
#include "PPM.h"
#include "getBytes.h"

using namespace std;
namespace fs = filesystem;

unsigned long PPM::fileSize() {
    /*in.seekg(0);
    unsigned char byte;
    while(byte != 0x0A) {
        byte = getBytesBE(in, 1);
    }*/
    return 0;
}

PPM::PPM(const string &) {
    this->path = path;
    in.open(path, ios::binary);
}

string PPM::getInfo() {
    fs::path p = fs::path(path);
    string message = "file name: " + p.filename().string() +
                     "\nfile extension: " + p.extension().string() +
                     "\nfile size (bytes): " + to_string(fileSize());
    return message;
}

bool PPM::checkMessage(const string &) {
    for (int j=0; j < 10; j++) {
        for (int i = 0; i < 10; i++) {
            cout << (char)getBytesBE(in,1) << " | ";
        }
        cout<<endl;
    }
    //TODO co to się staneło tutaj
    return false;
}

void PPM::encryptMessage(const string &) {

}

string PPM::decryptMessage() {
    return "";
}