#include <cstring>
#include <algorithm>
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

static inline int min(int a, int b, int c) {
    return std::min(std::min(a, b), c);
}

unsigned char *PNG::filter(unsigned char *array) {
    unsigned int width = imageWidth();
    unsigned int height = pixelArraySize()/imageWidth();
    unsigned char paddedArray[height + 1][width + 1];
    memset(&paddedArray[0], 0, (width + 1) * sizeof(unsigned char));
    for (unsigned int i = 0; i < height; i++) {
        paddedArray[i + 1][0] = 0;
        copy(array + (i * width), array + (i * width) + width, paddedArray[i + 1] + 1);
    }
    vector<size_t> rowYield;
    rowYield.resize(5);
    auto *filteredArray = new unsigned char[pixelArraySize() + height];
    for (unsigned int y = 0; y < height; y++) {
        memset(rowYield.data(), 0, 5 * sizeof(size_t));
        for (unsigned int x = 0; x < width; x++) {
            rowYield[0] += (char) paddedArray[y + 1][x + 1];    //none
            rowYield[1] += (char) paddedArray[y + 1][x + 1] - paddedArray[y + 1][x];    //sub
            rowYield[2] += (char) paddedArray[y + 1][x + 1] - paddedArray[y][x + 1];    //up
            rowYield[3] += (char) paddedArray[y + 1][x + 1] - (paddedArray[y + 1][x] + paddedArray[y][x + 1]) / 2;  //avg
            int v = paddedArray[y][x + 1] + paddedArray[y + 1][x] - paddedArray[y][x];
            rowYield[4] += (char) paddedArray[y + 1][x + 1] - min(v - paddedArray[y + 1][x], v - paddedArray[y][x + 1], v - paddedArray[y][x]);  //paeth
        }
        switch (distance(rowYield.begin(), min_element(rowYield.begin(), rowYield.end())))
        {
            default:
            case 0:     //none
                filteredArray[y * width + y] = 0;
                copy(&paddedArray[y + 1][1], &paddedArray[y + 1][width + 1], &filteredArray[(y * width) + (y + 1)]);
                break;
            case 1:     //sub
                filteredArray[y * width + y] = 1;
                for (unsigned int x = 0; x < width; x++) {
                    filteredArray[(y * width) + (y + 1) + x] = paddedArray[y + 1][x + 1] - paddedArray[y + 1][x];
                }
                break;
            case 2:     //up
                filteredArray[y * width + y] = 2;
                for (unsigned int x = 0; x < width; x++) {
                    filteredArray[(y * width) + (y + 1) + x] = paddedArray[y + 1][x + 1] - paddedArray[y][x + 1];
                }
                break;
            case 3:     //avg
                filteredArray[y * width + y] = 3;
                for (unsigned int x = 0; x < width; x++) {
                    filteredArray[(y * width) + (y + 1) + x] = paddedArray[y + 1][x + 1] - ((paddedArray[y + 1][x] + paddedArray[y][x + 1]) / 2);
                }
                break;
            case 4:     //paeth
                filteredArray[y * width + y] = 4;
                for (unsigned int x = 0; x < width; x++) {
                    int v = paddedArray[y][x + 1] + paddedArray[y + 1][x] - paddedArray[y][x];
                    filteredArray[(y * width) + (y + 1) + x] = paddedArray[y + 1][x + 1] - min(v - paddedArray[y + 1][x], v - paddedArray[y][x + 1], v - paddedArray[y][x]);
                }
                break;
        }
    }
    return filteredArray;
}

unsigned char *PNG::reverseFilter(unsigned char *array) {
    unsigned int width = imageWidth();
    unsigned int height = pixelArraySize() / imageWidth();
    auto *unfilteredArray = new unsigned char[width * height];
    unsigned char paddedArray[height + 1][width + 1];
    memset(&paddedArray, 0, (width + 1) * sizeof(unsigned char));
    for (unsigned int y = 0; y < height; y++) {
        paddedArray[y + 1][0] = 0;
        copy(&array[(y * width) + (y + 1)], &array[(y + 1) * (width + 1)], &paddedArray[y + 1][1]);
    }
    for (unsigned int y = 0; y < height; y++) {
        switch (array[y * (width + 1)])
        {
            case 0:     //none
                break;
            case 1:     //sub
                for (unsigned int x = 0; x < width; x++)
                {
                    paddedArray[y + 1][x + 1] += paddedArray[y + 1][x];
                }
                break;
            case 2:     //up
                for (unsigned int x = 0; x < width; x++) {
                    paddedArray[y + 1][x + 1] += paddedArray[y][x + 1];
                }
                break;
            case 3:     //avg
                for (unsigned int x = 0; x < width; x++) {
                    paddedArray[y + 1][x + 1] += (paddedArray[y][x + 1] + paddedArray[y + 1][x]) / 2;
                }
                break;
            case 4:     //paeth
                for (unsigned int x = 0; x < width; x++) {
                    int v = paddedArray[y][x + 1] + paddedArray[y + 1][x] - paddedArray[y][x];
                    paddedArray[y + 1][x + 1] += min(v - paddedArray[y + 1][x], v - paddedArray[y][x + 1], v - paddedArray[y][x]);
                }
                break;
        }
    }
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            copy(&paddedArray[y + 1][1], &paddedArray[y + 1][width + 1], &unfilteredArray[y * width]);
        }
    }
    return unfilteredArray;
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