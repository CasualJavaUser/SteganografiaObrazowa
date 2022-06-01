#ifndef STEGANOGRAFIAOBRAZOWA_BITOPS_H
#define STEGANOGRAFIAOBRAZOWA_BITOPS_H

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

long getBytes(ifstream &in, const int &count) {
    long out = 0;
    unsigned char byte;
    for (int i = 0; i < count; i++) {
        byte = (unsigned char) in.get();
        out = out | byte << 8 * i;
    }
    return out;
}

long getBytesReversed(ifstream& in, const int& count) {
    long out = 0;
    unsigned char byte;
    for(int i=count-1; i>=0; i--) {
        byte = (unsigned char)in.get();
        out = out | byte << 8 * i;
    }
    return out;
}

#endif //STEGANOGRAFIAOBRAZOWA_BITOPS_H