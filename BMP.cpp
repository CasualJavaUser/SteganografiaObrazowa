#include "BMP.h"
#include <filesystem>
#include "getBytes.h"

namespace fs = filesystem;

BMP::BMP(const string & path) {
    this->path = path;
    in.open(path, ios::binary);
}

unsigned int BMP::colorDepth() {
    in.seekg(28);
    return (int) getBytesSE(in, 2);
}

unsigned long long BMP::pixelArraySize() {
    in.seekg(18);
    return (getBytesSE(in, 4) * getBytesSE(in, 4));
}

unsigned long BMP::fileSize() {
    in.seekg(2);
    return getBytesSE(in, 4);
}

string BMP::getInfo() {
    fs::path p = fs::path(path);
    string message = "file name: " + p.filename().string() +
                     "\nfile extension: " + p.extension().string() +
                     "\nfile size (bytes): " + to_string(fileSize());
    in.ignore(12);
    message += "\nimage dimensions (pixels): " + to_string(getBytesSE(in, 4)) + " x " +
               to_string(getBytesSE(in, 4));
    message += "\ncolor depth: " + to_string(colorDepth());

    return message;
}

bool BMP::checkMessage(const string &message) {
    unsigned int bpp = colorDepth();
    if (bpp != 24 && bpp != 32) return false;  //unsupported type of bitmap

    unsigned long long bitCapacity = pixelArraySize() * 3;
    unsigned long long messageBitSize = message.size() * 8;

    if (bitCapacity < messageBitSize) return false;
    return true;
}

void BMP::encryptMessage(const string &message) {
    unsigned int bpp = colorDepth();
    unsigned long size = fileSize();
    in.seekg(10);
    long offset = getBytesSE(in, 4);  //The offset of the byte where the pixel pixelArray starts
    //unsigned char array[size];
    unsigned char * array = new unsigned char[size];

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
    for (int i = 0; i < size; i++) {
        out << array[i];
    }

    delete[] array;
    out.close();
}

string BMP::decryptMessage() {
    unsigned char byte = 0;
    string message;
    in.seekg(10);
    long offset = getBytesSE(in, 4);
    unsigned int bpp = colorDepth();
    unsigned long long size = pixelArraySize();
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