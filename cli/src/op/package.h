#pragma once

#include <filesystem>
#include <ostream>
#include <span>
#include <vector>

namespace op {
    std::vector<std::filesystem::path> listPackagedFiles(
        std::span<const std::filesystem::path> include,
        std::ostream &cerr,
        const std::filesystem::path &workingDirectory);

    int package(const std::filesystem::path &packagePath, const std::filesystem::path &workingDirectory, std::span<const std::filesystem::path> include);
}
