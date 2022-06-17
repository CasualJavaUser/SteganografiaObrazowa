#include <iostream>
#include <filesystem>
#include "BMP.h"
#include "PNG.h"
#include "PPM.h"

using namespace std;
namespace fs = filesystem;

const string extensions[] = {".bmp",".png", ".ppm"};

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
    fs::path p = fs::path(path);
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
    if(fs::is_directory(path)) {
        cout<<"Error: no file found.";
        return false;
    }
    if(!fs::exists(path)) {
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

bool checkPerms(const string& path) {
    ifstream in;
    in.open(path, ios::binary);
    if(!in.is_open()) {
        cout<<"Error: cannot open file: " + path;
        return false;
    }
    return true;
}

/*string temp(unsigned long bytes) {
    unsigned char byte;
    string s;
    for(int i=0; i<4; i++) {
        byte = (bytes>>8*i) & 0b11111111;
        s+=(char)byte;
    }
    return s;
}*/

/**
 * Gives information about the given file.
 * @param path - file path;
 * @param in - file stream that is used to read the file.
 * @return string containing information about the file.
 */

/*string getInfo(const fs::path& path, ifstream& in) {
    string message;
    if (path.extension().string() == extensions[0]) message = BMP::getInfo(path, in);
    else if (path.extension().string() == extensions[1]) message = PNG::getInfo(path, in);
    else if (path.extension().string() == extensions[2]) message = GIF::getInfo(path, in);

    return message;
}*/

string getInfo(const fs::path & path) {
    string ext = path.extension().string();
    if (ext == extensions[0]) return BMP(path.string()).getInfo();
    else if (ext == extensions[1]) return PNG(path.string()).getInfo();
    else if (ext == extensions[2]) return PPM(path.string()).getInfo();
    return "";
}

/**
 * Check whether the given message can be either encrypted in or decrypted from the file.
 * @param message - the message to encrypt or decrypt.
 * @param extension - the extension of the file.
 * @param in - the file stream that is used to read the file.
 * @return true if it's possible to encrypt or decrypt the message from the given file.
 */

/*bool checkMessage(const string& message, const string& extension, ifstream& in) {
    bool b = false;
    if(extension == extensions[0]) b = BMP::checkMessage(message, in);
    else if(extension == extensions[1]) b = PNG::checkMessage(message, in);
    return b;
}*/

bool checkMessage (const string& message, const fs::path& path) {
    bool b = false;
    string ext = path.extension().string();
    if (ext == extensions[0]) b = BMP(path.string()).checkMessage(message);
    else if (ext == extensions[1]) b = PNG(path.string()).checkMessage(message);
    else if (ext == extensions[2]) b = PPM(path.string()).checkMessage(message);
    return b;
}

/**
 * Encrypts the message in the file.
 * @param message - the message to be encrypted.
 * @param extension - the extension of the file.
 * @param path - file path.
 * @param in - the file stream that is used to read the file.
 */

void encryptMessage (const string& message, const fs::path& path) {
    if(path.extension() == extensions[0]) (BMP(path.string())).encryptMessage(message);
    else if(path.extension() == extensions[1]) (PNG(path.string())).encryptMessage(message);
    else if(path.extension() == extensions[2]) (PPM(path.string())).encryptMessage(message);
}

/**
 * Decrypts a message from the file and returns it.
 * @param extension - the extension of the file.
 * @param in - the file stream that is used to read the file.
 * @return the message encrypted in the file.
 */

string decryptMessage(const fs::path& path) {
    string message;
    if(path.extension().string() == extensions[0]) message = BMP(path.string()).decryptMessage();
    else if(path.extension().string() == extensions[1]) message = PNG(path.string()).decryptMessage();
    else if(path.extension().string() == extensions[2]) message = PPM(path.string()).decryptMessage();
    return message;
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
    const string messageEncryptError = "Error: message cannot be encrypted in the given file.";
    const string encrypted = "The message has been encrypted.";

    string message = helpMessage;

    if (argc > 1) {
        string flag = argv[1];
        if (flag[0] == '-') {
            if (flag == "-h" || flag == "--help") {
                if(checkArguments(argc, 0)) {
                    message = helpMessage;
                } else message = useHelp;

            } else if (flag == "-i" || flag == "--info") {
                if(checkArguments(argc, 1) && checkFile(argv[2]) && checkExtension(argv[2]) && checkPerms(argv[2])) {
                    fs::path path = fs::path(argv[2]);
                    message = getInfo(path);
                } else message = useHelp;

            } else if (flag == "-e" || flag == "--encrypt") {
                if(checkArguments(argc, 2) && checkFile(argv[2]) && checkExtension(argv[2]) && checkPerms(argv[2])) {
                    fs::path path = fs::path(argv[2]);
                    string text = argv[3];
                    text += '\u0003';  //end of text
                    if(!checkMessage(text, path)) message = messageEncryptError + useHelp;
                    else {
                        encryptMessage(text, path);
                        message = encrypted;
                    }

                } else message = useHelp;

            } else if (flag == "-d" || flag == "--decrypt") {
                if(checkArguments(argc, 1) && checkFile(argv[2]) && checkExtension(argv[2]) && checkPerms(argv[2])) {
                    fs::path path = fs::path(argv[2]);
                    message = "Message: ";
                    message += decryptMessage(path);
                } else message = useHelp;

            } else if (flag == "-c" || flag == "--check") {
                if(checkArguments(argc, 2) && checkFile(argv[2]) && checkExtension(argv[2]) && checkPerms(argv[2])) {
                    fs::path path = fs::path(argv[2]);
                    string text = argv[3];
                    text += '\u0003';  //end of text
                    if(checkMessage(text, path)) message = messageIsPossible;
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
