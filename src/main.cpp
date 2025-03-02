#include <iostream>
#include <csignal>
#include <thread>
#include <vector>
#include "../include/ProcessManager.h"
#include "../include/GuiManager.h"
#include "../include/globals.h" 

std::string onlyFindString;
std::string pid;

void loadArgs(int argc, char* argv[]);
bool pidExistVerify(int pid);
bool validatePid(std::vector<std::string>* strings);

int main(int argc, char* argv[]) {
    std::vector<std::string> extractedPath;
    int currentLine = 1;
    int maxLineLength = 20;

    loadArgs(argc, argv);
    if (!validatePid(&extractedPath)) return EXIT_FAILURE;

    GuiManager gui(&currentLine, &maxLineLength, &extractedPath);
    std::thread guiThread(&GuiManager::run, &gui);
    guiThread.join();

    return EXIT_SUCCESS;
}

bool pidExistVerify(int pid) {
    return kill(pid, 0) != 0;
}

bool validatePid(std::vector<std::string>* strings) {
    std::string mapPath = "/proc/" + pid + "/maps";
    try {
        if (pid.empty()) throw std::invalid_argument("No PID provided");
        int pidVal = std::stoi(pid);
        if (pidExistVerify(pidVal)) {
            std::cerr << "Invalid PID: " << pidVal << std::endl;
            return false;
        }
        return fileManager(mapPath, onlyFindString, strings);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

void loadArgs(int argc, char* argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "p:f:")) != -1) {
        switch (opt) {
            case 'p': pid = optarg; break;
            case 'f': onlyFindString = optarg; break;
            default:
                std::cerr << "Usage: " << argv[0] << " -p <PID> [-f <filter>]\n";
                exit(EXIT_FAILURE);
        }
    }
    if (pid.empty()) {
        std::cerr << "Missing -p option\n";
        exit(EXIT_FAILURE);
    }
}