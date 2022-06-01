#ifndef STEGANOGRAFIAOBRAZOWA_PNG_H
#define STEGANOGRAFIAOBRAZOWA_PNG_H

#endif //STEGANOGRAFIAOBRAZOWA_PNG_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include "getBytes.h"

using namespace std;
namespace fs = filesystem;

namespace PNG {
    long fileSize(ifstream& in) {
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

    string getInfo(const fs::path& path, ifstream& in) {
        string message = "\nfile size (bytes): " + to_string(fileSize(in));
        in.seekg(16);
        message += "\nimage dimensions (pixels): " + to_string(getBytesReversed(in, 4)) + " x " + to_string(
                getBytesReversed(in, 4));
        return message;
    }
}