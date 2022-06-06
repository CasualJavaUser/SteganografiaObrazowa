#ifndef STEGANOGRAFIAOBRAZOWA_PNG_H
#define STEGANOGRAFIAOBRAZOWA_PNG_H

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

namespace PNG {
    unsigned long long pixelArraySize(ifstream& in);
    long fileSize(ifstream& in);
    string getInfo(const fs::path& path, ifstream& in);
}

#endif //STEGANOGRAFIAOBRAZOWA_PNG_H