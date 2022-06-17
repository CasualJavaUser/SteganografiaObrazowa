#ifndef STEGANOGRAFIAOBRAZOWA_BITOPS_H
#define STEGANOGRAFIAOBRAZOWA_BITOPS_H

#include <iostream>
#include <fstream>

using namespace std;

long getBytesSE(ifstream &in, const int &count);
long getBytesBE(ifstream& in, const int& count);

#endif //STEGANOGRAFIAOBRAZOWA_BITOPS_H