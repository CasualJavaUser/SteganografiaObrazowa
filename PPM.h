#ifndef STEGANOGRAFIAOBRAZOWA_PPM_H
#define STEGANOGRAFIAOBRAZOWA_PPM_H

#include <iostream>
#include "Image.h"

using namespace std;

class PPM : Image {
private:
    unsigned long fileSize();
    unsigned long imageWidth();
    unsigned long imageHeight();

    /**
     * Gives information about the type of the .ppm format of the file.
     * @return 3 if the file is represented in ASCII characters or 6 if the format is of the raw/plain type.
     */
    unsigned char getType();
    unsigned char colorDepth();
public:
    PPM(const string &);
    string getInfo() override;
    bool checkMessage(const string &) override;
    void encryptMessage(const string &) override;
    string decryptMessage() override;
};

#endif //STEGANOGRAFIAOBRAZOWA_PPM_H
