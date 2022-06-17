#ifndef STEGANOGRAFIAOBRAZOWA_IMAGE_H
#define STEGANOGRAFIAOBRAZOWA_IMAGE_H

#include <iostream>
#include <fstream>

using namespace std;

class Image {
protected:
    string path;
    ifstream in;
public:
    virtual string getInfo() = 0;
    virtual bool checkMessage(const string&) = 0;
    virtual void encryptMessage(const string&) = 0;
    virtual string decryptMessage() = 0;
};


#endif //STEGANOGRAFIAOBRAZOWA_IMAGE_H
