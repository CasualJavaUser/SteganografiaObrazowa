
#ifndef STEGANOGRAFIAOBRAZOWA_BMP_H
#define STEGANOGRAFIAOBRAZOWA_BMP_H

#include <iostream>
#include <fstream>
#include "getBytes.h"
#include "Image.h"

using namespace std;

class BMP : Image {
private:
    /**
     * Reads the color depth value (bits per pixel) from the .bmp file.
     * @return the number of bits per pixel in the .bmp file.
     */
    unsigned int colorDepth();

    /**
     * Returns the number of pixels in the pixel array of the .bmp file.
     * @return the number of pixels in the pixel array.
     */
    unsigned long long pixelArraySize();

    /**
     * Returns the file size in bytes of the .bmp file.
     * @return the size in bytes of the .bmp file.
     */
    unsigned long fileSize();

public:
    BMP(const string&);

    /**
     * Gives information about the file.
     * @return string containing information about the file.
     */

    string getInfo();

    /**
     * Checks whether the given message can be either encrypted in or decrypted from the file.
     * @param message - the message to encrypt or decrypt.
     * @return true if it's possible to encrypt or decrypt the message from the given file.
     */
    bool checkMessage(const string&);

    /**
     * Encrypts the message in the file.
     * @param message - the message to be encrypted.
     */
    void encryptMessage (const string& message);

    /**
     * Decrypts a message from the file.
     * @return the message encrypted in the file.
     */
    string decryptMessage();
};

#endif //STEGANOGRAFIAOBRAZOWA_BMP_H