#include "PNG.h"
#include "getBytes.h"

unsigned int PNG::colorType(ifstream &in) {
    in.seekg(25);
    return getBytesReversed(in, 1);
}

string PNG::colorTypeName(ifstream& in) {
    unsigned int ct = colorType(in);
    switch (ct) {
        case 0: return "grayscale";
        case 2: return "RGB/truecolor";
        case 3: return "indexed";
        case 4: return "grayscale and alpha";
        case 6: return "truecolor and alpha";
        default: return "-";
    }
}

unsigned int PNG::colorDepth(ifstream &in) {
    unsigned int channels = 0;
    if (colorType(in) == 2) channels = 3;
    else if(colorType(in) == 6) channels = 4;
    in.seekg(24);
    return getBytesReversed(in, 1) * channels;
}

unsigned long long PNG::pixelArraySize(ifstream &in) {
    in.seekg(16);
    return getBytesReversed(in, 4) * getBytesReversed(in, 4);
}

unsigned long PNG::fileSize(ifstream &in) {
    unsigned long size = 8, offset;
    unsigned long chunkType = 0;
    in.seekg(8);
    while (chunkType != 1145980233) {  //IEND chunk
        offset = getBytesReversed(in, 4) + 4;
        chunkType = getBytes(in, 4);
        size += offset + 8;
        in.ignore(offset);
    }
    return size;
}

unsigned char *PNG::huffmanDec(unsigned char *&array) {
    return nullptr;
}

unsigned char *PNG::huffmanCom(unsigned char *&array) {
    return nullptr;
}

unsigned char *PNG::lzssDec(unsigned char *&array) {
    return nullptr;
}

unsigned char *PNG::lzssCom(unsigned char *&array) {
    return nullptr;
}

string PNG::getInfo(const fs::path &path, ifstream &in) {
    string message = "file name: " + path.filename().string() +
                     "\nfile extension: " + path.extension().string() +
                     "\nfile size (bytes): " + to_string(fileSize(in));
    in.seekg(16);
    message += "\nimage dimensions (pixels): " + to_string(getBytesReversed(in, 4)) + " x " +
            to_string(getBytesReversed(in, 4));
    message += "\ncolor type: " + colorTypeName(in);
    in.seekg(24);
    message += "\nbits per channel: " + to_string(getBytesReversed(in, 1));
    return message;
}

bool PNG::checkMessage(const string &message, ifstream &in) {
    unsigned int ct = colorType(in);
    if (ct != 2 && ct != 6) return false;  //unsupported type of png

    unsigned long long bitCapacity = pixelArraySize(in) * 3;
    unsigned long long messageBitSize = message.size() * 8;

    if (bitCapacity < messageBitSize) return false;
    return true;
}