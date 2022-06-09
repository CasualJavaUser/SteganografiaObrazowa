#ifndef STEGANOGRAFIAOBRAZOWA_GIF_H
#define STEGANOGRAFIAOBRAZOWA_GIF_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include "Image.h"

using namespace std;
namespace fs = filesystem;

class GIF : Image {
    GIF(const string&);
    unsigned long fileSize();
    string getInfo();
};
#endif //STEGANOGRAFIAOBRAZOWA_GIF_H
