#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

const string extensions[] = {".bmp",".png"};

/**
 * Returns true if the number of arguments is equal to the expected number.
 * Otherwise, returns false and prints error message.
 * @param argc - the number of arguments.
 * @param expected - the expected number of arguments.
 * @return true if the number of arguments is equal to the expected number.
 */

bool checkArguments(const int& argc, const int& expected) {
    if(argc != expected+2) cout<<"Error: expected " + to_string(expected) + " argument(s)";
    return argc == expected+2;
}

/**
 * Returns true if the file's format is supported.
 * Othewise, returns false and prints an error message.
 * @param path - file path.
 * @return true if the file's format is supported.
 */

bool checkExtension(const string& path) {
    filesystem::path p = filesystem::path(path);
    string extension = p.extension().string();
    string message = "Error: File format is not supported. Supported formats: ";

    for(const string& e : extensions) {
        if(extension == e) {
            return true;
        }
        message += e + " ";
    }
    cout << message;
    return false;
}

/**
 * Returns true if the given path leads to an existing file.
 * Otherwise, returns false and prints an error message.
 * @param path - file path.
 * @return true if the path leads to an existing file.
 */

bool checkFile(const string& path) {
    if(filesystem::is_directory(path)) {
        cout<<"Error: no file found.";
        return false;
    }
    if(!filesystem::exists(path)) {
        cout<<"Error: cannot find file: " + path;
        return false;
    }
    return true;
}

/**
 * Returns true if the file can be opened.
 * Otherwise returns false and prints an error message.
 * @param path - file path.
 * @param in - the file stream that is used to open the file.
 * @return true if the file can be opened.
 */

bool checkPerms(const string& path, ifstream& in) {
    in.open(path, ios::binary);
    if(!in.is_open()) {
        cout<<"Error: cannot open file: " + path;
        return false;
    }
    return true;
}

/**
 * Reads the given number of bytes.
 * @param in - file stream from which the bytes are read.
 * @param count - the number of bytes.
 * @return the given number of bytes from the stream in a single long value.
 */

long readBytes(ifstream& in, const int& count) {
    long out;
    unsigned char byte;
    for(int i=0; i<count; i++) {
        byte = (unsigned char)in.get();
        out = out | byte << 8 * i;
    }
    return out;
}

/**
 * Reads the color depth value (bits per pixel) from the .bmp file.
 * @param in - file stream that is used to read the file.
 * @return the number of bits per pixel in the .bmp file.
 */

int bmpColorDepth(ifstream& in) {
    in.seekg(28);
    return readBytes(in, 2);
}

/**
 * Check whether the given message can be either encrypted in or decrypted from the file.
 * @param message - the message to encrypt or decrypt.
 * @param in - the file stream that is used to read the file.
 * @return true if it's possible to encrypt or decrypt the message from the given file.
 */

bool checkMessage(const string& message, ifstream& in) {
    int bpp = bmpColorDepth(in);
    if(bpp != 24 && bpp != 32) return false;  //unsupported type of bitmap

    in.seekg(18);
    int width = readBytes(in, 4);
    int height = readBytes(in, 4);
    unsigned long long bitCapacity = width * height * 3;
    unsigned long long messageBitSize = message.size() * 8 + 8;

    if(bitCapacity < messageBitSize) return false;
    return true;
}

int main(int argc, const char* argv[]) {
    const string helpMessage = "Usage: steg [options] arguments\n"
                               "Options:\n"
                               "-h --help\t\t\tDisplay this information.\n"
                               "-i --info <file>\t\tChecks the file's format and displays more information about it.\n"
                               "-e --encrypt <file> <message>\tEncrypts the message in the file.\n"
                               "-d --decrypt <file>\t\tDecrypts a message from the file.\n"
                               "-c --check <file> <message>\tChecks if the given message can be encrypted in or decrypted from the file.\n\n"
                               "Supported image formats: .bmp, .png";
    const string noSuchFlag = "Error: nrecognized option ";
    const string useHelp = "\nUse -h or --help for more information.";
    const string messageIsPossible = "The given message can be either encrypted or decrypted from the file.";
    const string messageNotPossible = "The given message can be neither encrypted nor decrypted from the file.";

    string message = helpMessage;

    if (argc > 1) {
        string flag = argv[1];
        if (flag[0] == '-') {
            ifstream in;
            if (flag == "-h" || flag == "--help") {
                if(checkArguments(argc, 0)) {
                    message = helpMessage;
                } else message = useHelp;

            } else if (flag == "-i" || flag == "--info") {
                if(checkArguments(argc, 1) && checkFile(argv[2]) && checkExtension(argv[2]) && checkPerms(argv[2], in)) {
                    unsigned char byte;
                    unsigned long info = 0;
                    filesystem::path path = filesystem::path(argv[2]);

                    message = "file name: " + path.filename().string() +
                              "\nfile extension: " + path.extension().string();

                    in.ignore(2);
                    info = readBytes(in, 4);  //4
                    message += "\nfile size (bytes): " + to_string(info);

                    in.ignore(12);
                    info = readBytes(in, 4);
                    message += "\nimage dimensions: " + to_string(info) + " x ";

                    info = readBytes(in, 4);  //26
                    message += to_string(info);

                } else message = useHelp;

            } else if (flag == "-e" || flag == "--encrypt") {
                if(checkArguments(argc, 2) && checkFile(argv[2]) && checkExtension(argv[2]) && checkPerms(argv[2], in)) {
                    message = "enrypt"; //TODO

                } else message = useHelp;

            } else if (flag == "-d" || flag == "--decrypt") {
                if(checkArguments(argc, 1) && checkFile(argv[2]) && checkExtension(argv[2]) && checkPerms(argv[2], in)) {
                    message = "decrypt"; //TODO

                } else message = useHelp;

            } else if (flag == "-c" || flag == "--check") {
                if(checkArguments(argc, 2) && checkFile(argv[2]) && checkExtension(argv[2]) && checkPerms(argv[2], in)) {
                    if(checkMessage(argv[3], in)) message = messageIsPossible;
                    else message = messageNotPossible;

                } else message = useHelp;

            } else {
                message = noSuchFlag + flag + useHelp;
            }

        }
    }
    cout << message;

    return 0;
}
