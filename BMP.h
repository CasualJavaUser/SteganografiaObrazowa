
#ifndef STEGANOGRAFIAOBRAZOWA_BMP_H
#define STEGANOGRAFIAOBRAZOWA_BMP_H

#endif //STEGANOGRAFIAOBRAZOWA_BMP_H

#include "getBytes.h"

using namespace std;
namespace fs = filesystem;

namespace BMP {
    /**
     * Reads the color depth value (bits per pixel) from the .bmp file.
     * @param in - file stream that is used to read the file.
     * @return the number of bits per pixel in the .bmp file.
     */

    int colorDepth(ifstream& in) {
        in.seekg(28);
        return (int)getBytes(in, 2);
    }

    /**
     * Returns the number of pixels in the pixel array of the .bmp file.
     * @param in - file stream that is used to read the file.
     * @return the number of pixels in the pixel array.
     */

    unsigned long long pixelArraySize(ifstream& in) {
        in.seekg(18);
        return (getBytes(in,4) * getBytes(in,4));
    }

    /**
     * Returns the file size in bytes of the .bmp file.
     * @param in - file stream that is used to read the file.
     * @return the size in bytes of the .bmp file.
     */

    long fileSize(ifstream& in) {
        in.seekg(2);
        return getBytes(in, 4);
    }

    /**
     * Gives information about the given file.
     * @param path - file path;
     * @param in - file stream that is used to read the file.
     * @return string containing information about the file.
     */

    string getInfo(const fs::path& path, ifstream& in) {
        string message = "file name: " + path.filename().string() +
                         "\nfile extension: " + path.extension().string() +
                         "\nfile size (bytes): " + to_string(fileSize(in));
        in.ignore(12);
        message += "\nimage dimensions (pixels): " + to_string(getBytes(in, 4)) + " x " + to_string(getBytes(in, 4));

        return message;
    }

    /**
     * Check whether the given message can be either encrypted in or decrypted from the file.
     * @param message - the message to encrypt or decrypt.
     * @param extension - the extension of the file.
     * @param in - the file stream that is used to read the file.
     * @return true if it's possible to encrypt or decrypt the message from the given file.
     */

    bool checkMessage(const string& message, const string& extension, ifstream& in) {
        int bpp = colorDepth(in);
        if (bpp != 24 && bpp != 32) return false;  //unsupported type of bitmap

        unsigned long long bitCapacity = pixelArraySize(in) * 3;
        unsigned long long messageBitSize = message.size() * 8;

        if (bitCapacity < messageBitSize) return false;
        return true;
    }

    /**
     * Encrypts the message in the file.
     * @param message - the message to be encrypted.
     * @param path - file path.
     * @param in - the file stream that is used to read the file.
     */

    void encryptMessage (const string& message, const string& path, ifstream& in) {
        int bpp = colorDepth(in);
        long size = fileSize(in);
        in.seekg(10);
        int offset = getBytes(in, 4);  //The offset of the byte where the pixel pixelArray starts
        unsigned char array[size];

        in.seekg(0);
        for (int i = 0; i < size; ++i) {
            array[i] = in.get();
        }

        in.close();

        int wi = 0;
        for (int i = 0; i < message.size(); i++, wi++) {
            if (i % 4 == 0 && bpp == 32) wi++;  //skip alpha
            for (int j = 0; j < 8; j++) {
                array[offset + wi * 8 + j] &= 0b11111110;
                array[offset + wi * 8 + j] |= (message[i] >> j) & 1;
            }
        }

        ofstream out;
        out.open(path, ios::binary);
        for (unsigned char c: array) {
            out << c;
        }
        out.close();
    }

    /**
     * Decrypts a message from the file and returns it.
     * @param extension - the extension of the file.
     * @param in - the file stream that is used to read the file.
     * @return the message encrypted in the file.
     */

    string decryptMessage(ifstream& in) {
        unsigned char byte = 0;
        string message;
        in.seekg(10);
        int offset = getBytes(in, 4);
        int bpp = colorDepth(in);
        unsigned long long size = pixelArraySize(in);
        in.seekg(offset);
        int bits = 0;
        for (int i = 0; i < size; i++) {
            if (i % 4 == 0 && bpp == 32) {  //skip alpha
                in.ignore(1);
                i++;
            }

            unsigned char bit = in.get() & 1;
            if (bit != 0) byte |= 1 << bits;
            bits++;

            if (bits == 8) {
                if (byte == '\u0003') break;
                message += (char) byte;
                byte = 0;
                bits = 0;
            }
        }
        return message;
    }
}