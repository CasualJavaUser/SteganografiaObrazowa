#ifndef STEGANOGRAFIAOBRAZOWA_PNG_H
#define STEGANOGRAFIAOBRAZOWA_PNG_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include "Image.h"

using namespace std;
namespace fs = filesystem;

class PNG : Image {
private:
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
public:
    PNG(const string&);
    string getInfo() override;
    bool checkMessage(const string&) override;
    void encryptMessage(const string &) override;
    string decryptMessage() override;
};

#endif //STEGANOGRAFIAOBRAZOWA_PNG_H