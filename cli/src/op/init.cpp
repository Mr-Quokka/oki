#include "init.h"
#include "../cli/ExitStatuses.h"
#include "../io/oki.h"
#include "../make/CCompilatorStrategy.h"
#include "../make/CppCompilatorStrategy.h"
#include "../make/SourceFactory.h"
#include <cstring>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

namespace op {
    int init(
        const InitOptions &options,
        const fs::path &workingDirectory) {
        std::ofstream manifest{workingDirectory / OKI_MANIFEST_FILE};
        if (!manifest) {
            std::cerr << "Cannot write manifest: " << strerror(errno);
            return ERR_CANT_CREATE;
        }
        manifest << "[package]\n"
                    "name = \""
                 << options.projectName << "\"\n";
        manifest << "version = \"0.1.0\"\n";
        manifest << "kind = \"" << options.kind << "\"\n";
        if (options.lib) {
            manifest << "\n[lib.c]\n";
            manifest << "export = \"src\"\n";
            manifest << "build-static = \"make build" << fs::path::preferred_separator << options.projectName << ".a\"\n";
            manifest << "static-link = \"build" << fs::path::preferred_separator << options.projectName << ".a\"\n";
        }

        manifest << "\n[dependencies]\n";

        std::unique_ptr<make::CompilatorStrategy> strategy;
        strategy = make::SourceFactory::fromKind(options.kind);

        fs::path src{workingDirectory / "src"};
        fs::create_directories(src);

        if (!options.lib && strategy != nullptr) {
            if (!fs::exists(src / strategy->getMainFileName())) {
                std::ofstream main{src / strategy->getMainFileName()};
                if (!main) {
                    std::cerr << "Cannot write main source file: " << strerror(errno);
                    return ERR_CANT_CREATE;
                }
                strategy->writeSampleMain(main);
            }
        }
        std::cout << "Created `" << options.projectName << "` package\n";
        return OK;
    }
}