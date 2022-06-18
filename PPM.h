#ifndef STEGANOGRAFIAOBRAZOWA_PPM_H
#define STEGANOGRAFIAOBRAZOWA_PPM_H

#include <iostream>
#include "Image.h"

using namespace std;

class PPM : Image {
private:
    /**
     * Returns the file size in bytes.
     * @return the size in bytes.
     */
    unsigned long fileSize();

    /**
     * Returns the pixel width of the image.
     * @return the pixel width of the image.
     */
    unsigned long imageWidth();

    /**
     * Returns the pixel height of the image.
     * @return the pixel height of the image.
     */
    unsigned long imageHeight();

    /**
     * Gives information about the type of the .ppm format of the file.
     * @return 3 if the file is represented in ASCII characters or 6 if the format is of the raw/plain type.
     */
    unsigned char getType();

    /**
     * Returns the color depth of the image which is the max value for each color channel (RGB).
     * @return the color depth of the image.
     */
    unsigned char colorDepth();
public:
    PPM(const string &);

    /**
     * Gives information about the file.
     * @return string containing information about the file.
     */
    string getInfo() override;

    /**
     * Checks whether the given message can be either encrypted or decrypted from the file.
     * @param message - the message to encrypt or decrypt.
     * @return true if it's possible to encrypt or decrypt the message from the given file.
     */
    bool checkMessage(const string &) override;

    /**
     * Encrypts the message in the file.
     * @param message - the message to be encrypted.
     */
    void encryptMessage(const string &) override;

    /**
     * Decrypts a message from the file.
     * @return the message encrypted in the file.
     */
    string decryptMessage() override;
};

#endif //STEGANOGRAFIAOBRAZOWA_PPM_H
