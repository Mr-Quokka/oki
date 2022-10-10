#include "options.h"

#include <iostream>
#include <cstring>

void invalidUsage(std::ostream &os) {
    std::cerr << "Invalid usage. Hint: ";
}

void help(std::ostream &os) {
    os << "help: Show this help\n";
    os << "list: List available packages\n";
    os << "install: Install a new package\n";
}

CliAction parseArguments(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "See " << argv[0] << " help for all available actions.\n";
        exit(0);
    }
    if (strcmp("help", argv[1]) == 0) {
        help(std::cout);
    } else if (strcmp("list", argv[1]) == 0) {
        return ListAction{};
    } else if (strcmp("install", argv[1]) == 0) {
        if (argc < 3) {
            invalidUsage(std::cerr);
            std::cerr << "Add a package name after install.\n";
            exit(1);
        }
        return InstallAction{argv[2]};
    } else {
        exit(1);
    }
}
