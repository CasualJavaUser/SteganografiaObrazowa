#include "GIF.h"
#include "getBytes.h"

GIF::GIF(const string & path) {
    this->path = path;
    in.open(path, ios::binary);
}

unsigned long GIF::fileSize() {
    return 0;
}

string GIF::getInfo() {
    fs::path p = fs::path(path);
    string message = "file name: " + p.filename().string() +
                     "\nfile extension: " + p.extension().string() +
                     "\nfile size (bytes): " + to_string(fileSize());
    in.seekg(6);
    message += "\nimage dimensions (pixels): " + to_string(getBytesSE(in, 2)) + " x " +
               to_string(getBytesSE(in, 2));
    return message;
}