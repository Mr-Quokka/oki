#include "LocalRepository.h"

#include <iostream>

namespace fs = std::filesystem;

LocalRepository::LocalRepository(fs::path root) : root{std::move(root)} {}

void LocalRepository::createIfNotExists() {
    fs::create_directories(root);
}

std::vector<Package> LocalRepository::listPackages() {
    std::vector<Package> packages;
    for (const auto& file : fs::directory_iterator(root)) {
        packages.push_back(Package{file.path().filename().string()});
    }
    return packages;
}

void LocalRepository::download(std::string_view packageName, const fs::path& destination) {
    std::cerr << "TODO : downloading " << packageName << "\n";
}
