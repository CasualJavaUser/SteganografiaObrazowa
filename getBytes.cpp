#include "getBytes.h"

long getBytesSE(ifstream &in, const int &count) {
    long out = 0;
    unsigned char byte;
    for (int i = 0; i < count; i++) {
        byte = (unsigned char) in.get();
        out = out | byte << 8 * i;
    }
    return out;
}

long getBytesBE(ifstream& in, const int& count) {
    long out = 0;
    unsigned char byte;
    for(int i=count-1; i>=0; i--) {
        byte = (unsigned char)in.get();
        out = out | byte << 8 * i;
    }
    return out;
}