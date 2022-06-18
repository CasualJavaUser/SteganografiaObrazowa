#ifndef STEGANOGRAFIAOBRAZOWA_PNG_H
#define STEGANOGRAFIAOBRAZOWA_PNG_H

#include <iostream>
#include <fstream>
#include "Image.h"

using namespace std;

class PNG : Image {
private:
    /**
     * Returns an integer value representing the color type of the image (0, 2, 3, 4 or 6).
     * @return an integer value representing the color type of the image.
     */
    unsigned int colorType();

    /**
     * Returns a string containing the name of the color type
     * (grayscale, RGB/truecolor, indexed, grayscale and alpha, truecolor and alpha).
     * @return the name of the color type.
     */
    string colorTypeName();

    //unsigned int colorDepth();

    /**
     * Returns the size of the pixel array which is the number of pixels in the image.
     * @return the size of the pixel array.
     */
    unsigned long long pixelArraySize();

    /**
     * Returns the image pixel width.
     * @return the image pixel width.
     */
    unsigned int imageWidth();

    /**
     * Returns the file size.
     * @return the file size.
     */
    unsigned long fileSize();

    /**
     * Returns a char array containing all bytes from the image pixel array.
     * @return a char array containing all bytes from the image pixel array.
     */
    unsigned char* pixelArray();

    /**
     * Decompresses the image using the Huffman algorithm.
     * @return a char array containing bytes of the decompressed pixel array.
     */
    unsigned char* huffmanDec(unsigned char*);

    /**
     * Compresses the image using the Huffman algorithm.
     * @return a char array containing bytes of the compressed pixel array.
     */
    unsigned char* huffmanCom(unsigned char*);

    /**
     * Decompresses the image using the LZSS algorithm.
     * @return a char array containing bytes of the decompressed pixel array.
     */
    unsigned char* lzssDec(unsigned char*);

    /**
     * Compresses the image using the LZSS algorithm.
     * @return a char array containing bytes of the compressed pixel array.
     */
    unsigned char* lzssCom(unsigned char*);

    /**
     * Filters the PNG image.
     * @return a char array containing bytes of the filtered pixel array.
     */
    unsigned char* filter(unsigned char*);

    /**
     * Reverse filters the PNG image.
     * @return a char array containing bytes of the reverse filtered pixel array.
     */
    unsigned char* reverseFilter(unsigned char*);
public:
    PNG(const string&);

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
    bool checkMessage(const string&) override;

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

#endif //STEGANOGRAFIAOBRAZOWA_PNG_H