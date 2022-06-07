#ifndef STEGANOGRAFIAOBRAZOWA_PNG_H
#define STEGANOGRAFIAOBRAZOWA_PNG_H

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

namespace PNG {
    unsigned int colorType(ifstream& in);
    string colorTypeName(ifstream& in);
    unsigned int colorDepth(ifstream& in);
    unsigned long long pixelArraySize(ifstream& in);
    unsigned long fileSize(ifstream& in);
    unsigned char* huffmanDec(unsigned char* array);
    unsigned char* huffmanCom(unsigned char* array);
    unsigned char* lzssDec(unsigned char* array);
    unsigned char* lzssCom(unsigned char* array);
    unsigned char* reverseFilter(unsigned char* array);
    string getInfo(const fs::path& path, ifstream& in);
    bool checkMessage(const string& message, ifstream& in);
}

#endif //STEGANOGRAFIAOBRAZOWA_PNG_H