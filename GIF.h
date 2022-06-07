#ifndef STEGANOGRAFIAOBRAZOWA_GIF_H
#define STEGANOGRAFIAOBRAZOWA_GIF_H

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

namespace GIF {
    unsigned long fileSize(ifstream& in);
    string getInfo(const fs::path& path, ifstream& in);
}
#endif //STEGANOGRAFIAOBRAZOWA_GIF_H
