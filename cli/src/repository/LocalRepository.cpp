#include "LocalRepository.h"

#include <iostream>

namespace fs = std::filesystem;

namespace oki {
    LocalRepository::LocalRepository(fs::path root) : root{std::move(root)} {}

    void LocalRepository::createIfNotExists() {
        fs::create_directories(root);
    }

    std::vector<Package> LocalRepository::listPackages() {
        std::vector<Package> packages;
        for (const auto &file : fs::directory_iterator(root)) {
            packages.emplace_back(file.path().filename().string(), "");
        }
        return packages;
    }

    void LocalRepository::download(const Version &packageVersion, const fs::path &destination) {
        std::cerr << "TODO : downloading " << packageVersion.getIdentifier() << " at " << destination << "\n";
    }

    Package LocalRepository::getPackageInfo(std::string_view packageName) {
        std::cerr << "TODO : show " << packageName << "\n";
        throw std::logic_error{"Not implemented"};
    }

    std::string LocalRepository::getPackageURL(std::string_view packageName, std::string packageVersion) {
        std::cerr << "TODO : " << packageName << "/" << packageVersion << "\n";
        throw std::logic_error{"Not implemented"};
    }
}
