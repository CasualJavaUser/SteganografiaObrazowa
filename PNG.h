#ifndef STEGANOGRAFIAOBRAZOWA_PNG_H
#define STEGANOGRAFIAOBRAZOWA_PNG_H

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

namespace PNG {
    unsigned int colorType(ifstream&);
    string colorTypeName(ifstream&);
    unsigned int colorDepth(ifstream&);
    unsigned long long pixelArraySize(ifstream&);
    unsigned int imageWidth(ifstream&);
    unsigned long fileSize(ifstream&);
    unsigned char* pixelArray(ifstream&);
    unsigned char* huffmanDec(unsigned char*);
    unsigned char* huffmanCom(unsigned char*);
    unsigned char* lzssDec(unsigned char*);
    unsigned char* lzssCom(unsigned char*);
    unsigned char* reverseFilter(unsigned char*);
    string getInfo(const fs::path&, ifstream&);
    bool checkMessage(const string&, ifstream&);
}

#endif //STEGANOGRAFIAOBRAZOWA_PNG_H