#include "options.h"

#include <cstring>
#include <iostream>

#include "InstallAction.h"
#include "ListAction.h"
#include "MakefileAction.h"
#include "PublishAction.h"
#include "ReInstallAction.h"
#include "ShowAction.h"

namespace cli {
    void invalidUsage(std::ostream &os) {
        os << "Invalid usage. Hint: ";
    }

    void help(std::ostream &os) {
        os << "help: Show this help\n";
        os << "list: List available packages\n";
        os << "show: Show the informations of the package\n";
        os << "install: Install a new package\n";
        os << "reinstall: Install all the package of the manifest\n";
        os << "publish: Publish a new version of the current package\n";
        os << "makefile: Create a makefile\n";
    }

    std::unique_ptr<CliAction> parseArguments(int argc, char *argv[]) {
        if (argc < 2) {
            std::cerr << "See " << argv[0] << " help for all available actions.\n";
            exit(0);
        }
        if (strcmp("help", argv[1]) == 0) {
            help(std::cout);
            exit(0);
        } else if (strcmp("list", argv[1]) == 0) {
            return std::make_unique<ListAction>();
        } else if (strcmp("install", argv[1]) == 0) {
            if (argc < 3) {
                invalidUsage(std::cerr);
                std::cerr << "Add a package name after install.\n";
                exit(1);
            }
            return std::make_unique<InstallAction>(argv[2]);
        } else if (strcmp("show", argv[1]) == 0) {
            if (argc < 3) {
                invalidUsage(std::cerr);
                std::cerr << "Add a package name after show.\n";
                exit(1);
            }
            return std::make_unique<ShowAction>(argv[2]);
        } else if (strcmp("reinstall", argv[1]) == 0) {
            return std::make_unique<ReInstallAction>();
        } else if (strcmp("publish", argv[1]) == 0) {
            return std::make_unique<PublishAction>();
        } else if (strcmp("makefile", argv[1]) == 0) {
            if (argc > 3) {
                invalidUsage(std::cerr);
                std::cerr << "Too many arguments\n";
                exit(1);
            } else if (argc == 3) {
                return std::make_unique<MakefileAction>(argv[2]);
            } else {
                return std::make_unique<MakefileAction>();
            }
        } else {
            exit(1);
        }
    }
}
