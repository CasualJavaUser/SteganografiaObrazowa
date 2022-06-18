#include <filesystem>
#include "PPM.h"
#include "getBytes.h"

using namespace std;
namespace fs = filesystem;

unsigned long PPM::fileSize() {
    in.seekg(0);
    unsigned long size = 0;
    while (!in.eof()) {
        in.get();
        size++;
    }
    size--;  //minus eof

    in.close();
    in.open(path, ios::binary);

    return size;
}

unsigned long PPM::imageWidth() {
    in.seekg(3);

    unsigned char byte = 0;
    unsigned long width = 0;

    while(byte != 0x0A) {
        byte = getBytesBE(in, 1);
    }

    byte = getBytesBE(in, 1);
    while (byte != 0x20) {
        width *= 10;
        width += (byte - 48);
        byte = getBytesBE(in, 1);
    }
    return width;
}

unsigned long PPM::imageHeight() {
    in.seekg(3);

    unsigned char byte = 0;
    unsigned long height = 0;

    while(byte != 0x0A) {
        byte = getBytesBE(in, 1);
    }
    while(byte != 0x20) {  //space
        byte = getBytesBE(in, 1);
    }

    byte = getBytesBE(in, 1);
    while (byte != 0x0A) {
        height *= 10;
        height += (byte - 48);
        byte = getBytesBE(in, 1);
    }
    return height;
}

unsigned char PPM::getType() {
    in.seekg(1);
    return (getBytesBE(in,1))-48;
}

unsigned char PPM::colorDepth() {
    imageHeight();  //skip to image height

    unsigned char cd = 0;
    unsigned char byte = 0;

    byte = getBytesBE(in, 1);
    while (byte != 0x0A) {
        cd *= 10;
        cd += (byte - 48);
        byte = getBytesBE(in, 1);
    }
    return cd;
}

PPM::PPM(const string & path) {
    this->path = path;
    in.open(path, ios::binary);
}

string PPM::getInfo() {
    fs::path p = fs::path(path);
    string message = "file name: " + p.filename().string() +
                     "\nfile extension: " + p.extension().string() +
                     "\nfile size (bytes): " + to_string(fileSize()) +
                     "\nimage dimensions (pixels): " + to_string(imageWidth()) + " x " + to_string(imageHeight()) +
                     "\ncolor depth: " + to_string(colorDepth());
    if (getType() == 6) message += "\ntype: raw";
    else if (getType() == 3) message += "\ntype: ASCII";
    return message;
}

bool PPM::checkMessage(const string & message) {
    unsigned long long imageSize = imageWidth() * imageHeight();
    if (colorDepth() != 255) return false;
    if (getType() != 6 && getType() != 3) return false;

    unsigned long long bitCapacity = imageSize * 3;
    unsigned long long messageBitSize = message.size() * 8;

    if (bitCapacity < messageBitSize) return false;
    return true;
}



void PPM::encryptMessage(const string & message) {
    unsigned char type = getType();
    unsigned long long size = fileSize();
    unsigned char * array = new unsigned char[size];
    long long offset;

    in.seekg(0);
    for (int i = 0; i < size; ++i) {
        array[i] = in.get();
    }

    colorDepth();  //skip to color depth
    offset = in.tellg();
    in.close();

    if(type == 6) {  //raw
        for (int i = 0; i < message.size(); i++) {
            for (int j = 0; j < 8; j++) {
                array[offset + i * 8 + j] &= 0b11111110;
                array[offset + i * 8 + j] |= (message[i] >> j) & 1;
            }
        }
    } else {  //ascii
        long arrayIndex = offset;
        for (int i = 0; i < message.size(); i++) {
            for(int j = 0; j < 8; j++) {
                while (array[arrayIndex + 1] != 0x0A) { arrayIndex++; }
                array[arrayIndex] &= 0b11111110;
                array[arrayIndex] |= (message[i] >> j) & 1;
                arrayIndex++;
            }
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

string PPM::decryptMessage() {
    unsigned long long size = fileSize();
    unsigned char type = getType();
    unsigned char byte = 0;
    string message;
    long long offset;

    colorDepth();  //skip to color depth
    offset = in.tellg();

    in.seekg(offset);

    if(type == 6) {  //raw
        int bits = 0;
        for (int i = 0; i < size; i++) {
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
    } else {  //ASCII
        int bits = 0;
        for (int i = 0; i < size; i++) {
            unsigned char currentByte = (unsigned char) getBytesBE(in, 1);
            while (currentByte != 0x0A) {
                currentByte = getBytesBE(in, 1);
            }
            in.seekg((int)in.tellg()-2);
            unsigned char bit = in.get() & 1;
            in.ignore(2);

            if (bit != 0) byte |= 1 << bits;
            bits++;

            if (bits == 8) {
                if (byte == '\u0003') break;
                message += (char) byte;
                byte = 0;
                bits = 0;
            }
        }
    }

    return message;
}