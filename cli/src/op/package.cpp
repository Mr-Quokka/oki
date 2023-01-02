#include "package.h"
#include "../config/Manifest.h"
#include "../io/Archive.h"
#include "../io/oki.h"

namespace fs = std::filesystem;

namespace op {
    std::vector<fs::path> listPackagedFiles(std::span<const fs::path> include) {
        std::vector<fs::path> files;
        for (const auto &path : include) {
            if (fs::is_regular_file(path)) {
                files.emplace_back(path);
                continue;
            }
            for (const auto &dirEntry : fs::recursive_directory_iterator{path}) {
                if (dirEntry.is_regular_file()) {
                    files.emplace_back(dirEntry.path());
                }
            }
        }
        return files;
    }

    int package(const fs::path &packagePath, const fs::path &workingDirectory) {
        config::Manifest manifest = config::Manifest::fromFile(workingDirectory / OKI_MANIFEST_FILE);
        io::Compressor compressor{packagePath};
        std::vector<fs::path> include = listPackagedFiles(manifest.getInclude());
        include.emplace_back(workingDirectory / OKI_MANIFEST_FILE);
        compressor.compress(include, fs::current_path());
        return 0;
    }
}
