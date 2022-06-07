#include "GIF.h"
#include "getBytes.h"

unsigned long GIF::fileSize(ifstream &in) {
    return 3;
}

string GIF::getInfo(const fs::path &path, ifstream &in) {
    string message = "file name: " + path.filename().string() +
                     "\nfile extension: " + path.extension().string() +
                     "\nfile size (bytes): " + to_string(fileSize(in));
    in.seekg(6);
    message += "\nimage dimensions (pixels): " + to_string(getBytesSE(in, 2)) + " x " +
               to_string(getBytesSE(in, 2));
    return message;
}