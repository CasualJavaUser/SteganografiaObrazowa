#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

const string extensions[] = {".bmp",".png"};

bool checkArguments(int argc, int expected) {
    if(argc != expected+2) cout<<"Expected " + to_string(expected) + " argument(s)";
    return argc == expected+2;
}

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

bool checkPerms(const string& path, ifstream & in) {
    in.open(path, ios::binary);
    if(!in.is_open()) {
        cout<<"Error: cannot open file: " + path;
        return false;
    }
    return true;
}

long readBytes(ifstream & in, int count) {
    long out;
    unsigned char byte;
    for(int i=0; i<count; i++) {
        byte = (unsigned char)in.get();
        out = out | byte << 8 * i;
    }
    return out;
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

                    info = readBytes(in,4);  //26
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
                    message = "check"; //TODO

                } else message = useHelp;

            } else {
                message = noSuchFlag + flag + useHelp;
            }

        }
    }
    cout << message;

    return 0;
}
