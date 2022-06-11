#ifndef STEGANOGRAFIAOBRAZOWA_PNG_H
#define STEGANOGRAFIAOBRAZOWA_PNG_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include "Image.h"

using namespace std;
namespace fs = filesystem;

class PNG : Image {
public:
    PNG(const string&);
    unsigned int colorType();
    string colorTypeName();
    unsigned int colorDepth();
    unsigned long long pixelArraySize();
    unsigned int imageWidth();
    unsigned long fileSize();
    unsigned char* pixelArray();
    unsigned char* huffmanDec(unsigned char*);
    unsigned char* huffmanCom(unsigned char*);
    unsigned char* lzssDec(unsigned char*);
    unsigned char* lzssCom(unsigned char*);
    unsigned char* filter(unsigned char*);
    unsigned char* reverseFilter(unsigned char*);
    string getInfo();
    bool checkMessage(const string&);
};

#endif //STEGANOGRAFIAOBRAZOWA_PNG_H