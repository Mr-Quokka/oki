#pragma once

#include <filesystem>
#include <span>
#include <vector>

namespace op {
    std::vector<std::filesystem::path> listPackagedFiles(std::span<const std::filesystem::path> include);

    int package(const std::filesystem::path &packagePath, const std::filesystem::path &workingDirectory);
}
