#include "verify.h"
#include "../io/oki.h"
#include <algorithm>
#include <iostream>

namespace fs = std::filesystem;

namespace op {
    bool verify(const config::Manifest &manifest, std::span<const fs::path> include) {
        auto count = std::count_if(include.begin(), include.end(), [](const fs::path &path) {
            return path.filename() != OKI_MANIFEST_FILE && path.filename() != OKI_LOCK_FILE;
        });
        if (count == 0) {
            std::cerr << "No files to include\n";
            return false;
        }
        std::string name = manifest.getProjectName();
        if (name.empty()) {
            std::cerr << "Empty project name\n";
            return false;
        }
        if (name.length() > 64) {
            std::cerr << "Project name too long\n";
            return false;
        }
        for (char c : name) {
            if (!std::islower(c) && !std::isdigit(c) && c != '-' && c != '_') {
                std::cerr << "Invalid character `" << c << "` in project name: `" << name << "`\n";
                return false;
            }
        }
        return true;
    }
}
