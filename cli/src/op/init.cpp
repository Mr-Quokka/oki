#include "init.h"
#include "../cli/ExitStatuses.h"
#include "../io/oki.h"
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

        fs::path src{workingDirectory / "src"};
        fs::create_directories(src);
        if (options.kind == make::ProjectKind::C && !options.lib) {
            std::ofstream main{src / "main.c"};
            if (!main) {
                std::cerr << "Cannot write main source file: " << strerror(errno);
                return ERR_CANT_CREATE;
            }
            main << "#include <stdio.h>\n\n"
                 << "int main(void) {\n"
                 << "    printf(\"Hello world!\\n\");\n"
                 << "    return 0;\n"
                 << "}\n";
        } else if (options.kind == make::ProjectKind::Cpp && !options.lib) {
            std::ofstream main{src / "main.cpp"};
            if (!main) {
                std::cerr << "Cannot write main source file: " << strerror(errno);
                return ERR_CANT_CREATE;
            }
            main << "#include <iostream>\n\n"
                 << "int main() {\n"
                 << "    std::cout << \"Hello world!\\n\";\n"
                 << "    return 0;\n"
                 << "}\n";
        }

        std::cout << "Created `" << options.projectName << "` package\n";
        return OK;
    }
}