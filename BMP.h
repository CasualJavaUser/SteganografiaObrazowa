
#ifndef STEGANOGRAFIAOBRAZOWA_BMP_H
#define STEGANOGRAFIAOBRAZOWA_BMP_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include "getBytes.h"
#include "Image.h"

using namespace std;
namespace fs = filesystem;

class BMP : Image {
public:
    BMP(const string&);

    /**
     * Reads the color depth value (bits per pixel) from the .bmp file.
     * @param in - file stream that is used to read the file.
     * @return the number of bits per pixel in the .bmp file.
     */
    unsigned int colorDepth();

    /**
     * Returns the number of pixels in the pixel array of the .bmp file.
     * @param in - file stream that is used to read the file.
     * @return the number of pixels in the pixel array.
     */
    unsigned long long pixelArraySize();

    /**
     * Returns the file size in bytes of the .bmp file.
     * @param in - file stream that is used to read the file.
     * @return the size in bytes of the .bmp file.
     */
    unsigned long fileSize();

    /**
     * Gives information about the given file.
     * @param path - file path;
     * @param in - file stream that is used to read the file.
     * @return string containing information about the file.
     */

    string getInfo();

    /**
     * Check whether the given message can be either encrypted in or decrypted from the file.
     * @param message - the message to encrypt or decrypt.
     * @param extension - the extension of the file.
     * @param in - the file stream that is used to read the file.
     * @return true if it's possible to encrypt or decrypt the message from the given file.
     */
    bool checkMessage(const string&);

    /**
     * Encrypts the message in the file.
     * @param message - the message to be encrypted.
     * @param path - file path.
     * @param in - the file stream that is used to read the file.
     */
    void encryptMessage (const string& message);

    /**
     * Decrypts a message from the given file.
     * @param extension - the extension of the file.
     * @param in - the file stream that is used to read the file.
     * @return the message encrypted in the file.
     */
    string decryptMessage();
};

#endif //STEGANOGRAFIAOBRAZOWA_BMP_H