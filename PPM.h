#ifndef STEGANOGRAFIAOBRAZOWA_PPM_H
#define STEGANOGRAFIAOBRAZOWA_PPM_H

#include <iostream>
#include "Image.h"

using namespace std;

class PPM : Image {
private:
    unsigned long fileSize();
public:
    PPM(const string &);
    string getInfo() override;
    bool checkMessage(const string &) override;
    void encryptMessage(const string &) override;
    string decryptMessage() override;
};

#endif //STEGANOGRAFIAOBRAZOWA_PPM_H
