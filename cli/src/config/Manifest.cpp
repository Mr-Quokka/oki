#include "Manifest.h"

#include <toml.hpp>
#include <iostream>

namespace fs = std::filesystem;

constexpr static std::string_view DEPENDENCY_SECTION_NAME = "dependencies";

namespace config {
    Manifest Manifest::fromFile(fs::path fileName) {
        Manifest manifest;
        if (manifest.loadFileIfExists(fileName) == true) {
            return manifest;
        } else {
            throw ManifestException("This isn't an project using oki");
        }
    }

    std::unordered_map<std::string_view, std::string> Manifest::listDeclaredPackages() const {
        std::unordered_map<std::string_view, std::string> packages;
        for (const auto &dependency : *table[DEPENDENCY_SECTION_NAME].as_table()) {
            packages.emplace(dependency.first, dependency.second.as_string()->get());
        }
        return packages;
    }

    bool Manifest::addDeclaredPackage(std::string_view packageName, std::string_view version) {
        return addDependencySectionIfNotExists().insert_or_assign(packageName, version).second;
    }

    bool Manifest::addDeclaredPackage(std::string_view packageName, semver::Version &version) {
        return addDeclaredPackage(packageName, version.str());
    }

    toml::v3::table &Manifest::addDependencySectionIfNotExists() {
        auto it = table.find(DEPENDENCY_SECTION_NAME);
        if (it == table.end()) {
            table.insert(DEPENDENCY_SECTION_NAME, toml::v3::table{});
        }
        return *table[DEPENDENCY_SECTION_NAME].as_table();
    }

    bool Manifest::loadFileIfExists(fs::path fileName) {
        fs::path pwd = std::filesystem::current_path();
        fs::path parentPath = pwd;
        fs::path searchedFile = parentPath / fileName;
        while(parentPath != parentPath.parent_path()){
            if (fs::exists(searchedFile)) {
                table = toml::parse_file(searchedFile.c_str());
                return true;
            } else {
                parentPath = parentPath.parent_path();
                searchedFile = parentPath / fileName;
            }
        }
        return false;
    }

    void Manifest::saveFile(fs::path fileName) {
        std::ofstream os{fileName};
        os << *this;
    }

    std::ostream &operator<<(std::ostream &os, const Manifest &manifest) {
        os << manifest.table;
        return os;
    }

    ManifestException::ManifestException(std::string_view msg) : msg{msg} {}

    const char *ManifestException::what() const noexcept {
        return this->msg.c_str();
    }
}
