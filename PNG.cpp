#include "PNG.h"
#include "getBytes.h"

unsigned long long PNG::pixelArraySize(ifstream &in) {
    in.seekg(16);
    return getBytesReversed(in, 4) * getBytesReversed(in, 4);
}

long PNG::fileSize(ifstream &in) {
    long size = 8, offset;
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

string PNG::getInfo(const fs::path &path, ifstream &in) {
    string message = "file name: " + path.filename().string() +
                     "\nfile extension: " + path.extension().string() +
                     "\nfile size (bytes): " + to_string(fileSize(in));
    in.seekg(16);
    message += "\nimage dimensions (pixels): " + to_string(getBytesReversed(in, 4)) + " x " + to_string(
            getBytesReversed(in, 4));
    return message;
}