#include "package.h"
#include "../cli/ExitStatuses.h"
#include "../config/Manifest.h"
#include "../io/Archive.h"
#include "../io/oki.h"

namespace fs = std::filesystem;

namespace op {
    std::vector<fs::path> listPackagedFiles(
        std::span<const fs::path> include,
        std::ostream &cerr,
        const fs::path &workingDirectory) {
        std::vector<fs::path> files;
        for (const auto &path : include) {
            if (fs::is_regular_file(workingDirectory / path)) {
                files.emplace_back(path);
                continue;
            }
            try {
                for (const auto &dirEntry : fs::recursive_directory_iterator{workingDirectory / path}) {
                    if (dirEntry.is_regular_file()) {
                        files.emplace_back(dirEntry.path());
                    }
                }
            } catch (fs::filesystem_error &e) {
                cerr << e.what() << "\n";
            }
        }
        files.emplace_back(workingDirectory / OKI_MANIFEST_FILE);
        return files;
    }

    int package(const fs::path &packagePath, const fs::path &workingDirectory, std::span<const fs::path> include) {
        config::Manifest manifest = config::Manifest::fromFile(workingDirectory / OKI_MANIFEST_FILE);
        io::Compressor compressor{packagePath};
        compressor.compress(include, fs::current_path());
        return OK;
    }
}
