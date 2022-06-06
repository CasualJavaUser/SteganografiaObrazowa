#ifndef STEGANOGRAFIAOBRAZOWA_BITOPS_H
#define STEGANOGRAFIAOBRAZOWA_BITOPS_H

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

long getBytes(ifstream &in, const int &count);
long getBytesReversed(ifstream& in, const int& count);

#endif //STEGANOGRAFIAOBRAZOWA_BITOPS_H