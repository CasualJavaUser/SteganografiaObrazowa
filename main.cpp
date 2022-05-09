#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

const string extensions[] = {".bmp",".png"};

bool checkArguments(int argc, int expected) {
    if(argc != expected) cout<<"Expected " + to_string(expected) + " argument(s)";
    return argc == expected;
}

bool checkExtension(const string& path) {
    filesystem::path p = filesystem::path(path);
    string extension = p.extension().string();
    for(const string& e : extensions) {
        if(extension == e) return true;
    }
    return false;
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
            if (flag == "-h" || flag == "--help") {
                message = helpMessage;

            } else if (flag == "-i" || flag == "--info") {
                if(checkArguments(argc, 3)) {
                    message = "info"; //TODO

                } else message = useHelp;

            } else if (flag == "-e" || flag == "--encrypt") {
                if(checkArguments(argc, 4)) {
                    message = "enrypt"; //TODO

                } else message = useHelp;

            } else if (flag == "-d" || flag == "--decrypt") {
                if(checkArguments(argc, 3)) {
                    message = "decrypt"; //TODO

                } else message = useHelp;

            } else if (flag == "-c" || flag == "--check") {
                if(checkArguments(argc, 4)) {
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
