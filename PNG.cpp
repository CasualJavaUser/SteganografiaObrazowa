#include "PNG.h"
#include "getBytes.h"

PNG::PNG(const string & path) {
    this->path = path;
    in.open(path, ios::binary);
}

unsigned int PNG::colorType() {
    in.seekg(25);
    return getBytesBE(in, 1);
}

string PNG::colorTypeName() {
    unsigned int ct = colorType();
    switch (ct) {
        case 0: return "grayscale";
        case 2: return "RGB/truecolor";
        case 3: return "indexed";
        case 4: return "grayscale and alpha";
        case 6: return "truecolor and alpha";
        default: return "-";
    }
}

unsigned int PNG::colorDepth() {
    unsigned int channels = 0;
    if (colorType() == 2) channels = 3;
    else if(colorType() == 6) channels = 4;
    in.seekg(24);
    return getBytesBE(in, 1) * channels;
}

unsigned long long PNG::pixelArraySize() {
    in.seekg(16);
    return getBytesBE(in, 4) * getBytesBE(in, 4);
}

unsigned int PNG::imageWidth() {
    in.seekg(16);
    return getBytesBE(in, 4);
}

unsigned long PNG::fileSize() {
    unsigned long size = 8, offset;
    unsigned long chunkType = 0;
    in.seekg(8);
    while (chunkType != 0x49454E44) {  //IEND chunk
        offset = getBytesBE(in, 4) + 4;
        chunkType = getBytesBE(in, 4);
        size += offset + 8;
        in.ignore(offset);
    }
    return size;
}

unsigned char *PNG::pixelArray() {
    unsigned long size = 0;
    unsigned long chunkType = 0;
    in.seekg(8);
    while (chunkType != 0x49444154) {  //IDAT chunk
        in.ignore(size);
        size = getBytesBE(in, 4) + 4;
        chunkType = getBytesBE(in, 4);
    }

    in.ignore(2);
    size -= 6;
    unsigned char *array = new unsigned char[size];

    for (int i = 0; i < size; i++) {
        array[i] = (unsigned char)getBytesBE(in, 1);
    }

    return array;
}

unsigned char *PNG::huffmanDec(unsigned char *array) {
    return nullptr;
}

unsigned char *PNG::huffmanCom(unsigned char *array) {
    return nullptr;
}

unsigned char *PNG::lzssDec(unsigned char *array) {
    return nullptr;
}

unsigned char *PNG::lzssCom(unsigned char *array) {
    return nullptr;
}

unsigned char *PNG::reverseFilter(unsigned char *array) {
    return nullptr;
}

string PNG::getInfo() {
    fs::path p = fs::path(path);
    string message = "file name: " + p.filename().string() +
                     "\nfile extension: " + p.extension().string() +
                     "\nfile size (bytes): " + to_string(fileSize());
    in.seekg(16);
    message += "\nimage dimensions (pixels): " + to_string(getBytesBE(in, 4)) + " x " +
               to_string(getBytesBE(in, 4));
    message += "\ncolor type: " + colorTypeName();
    in.seekg(24);
    message += "\nbits per channel: " + to_string(getBytesBE(in, 1));
    return message;
}

bool PNG::checkMessage(const string &message) {
    unsigned int ct = colorType();
    if (ct != 2 && ct != 6) return false;  //unsupported type of png

    unsigned long long bitCapacity = pixelArraySize() * 3;
    unsigned long long messageBitSize = message.size() * 8;

    if (bitCapacity < messageBitSize) return false;
    return true;
}