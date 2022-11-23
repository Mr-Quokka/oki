#include "Manifest.h"

#include <toml.hpp>

namespace fs = std::filesystem;

constexpr static std::string_view DEPENDENCY_SECTION_NAME = "dependencies";

namespace config {
    Manifest Manifest::fromFile(fs::path fileName) {
        Manifest manifest;
        manifest.loadFileIfExists(fileName);
        return manifest;
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

    void Manifest::loadFileIfExists(fs::path fileName) {
        if (fs::exists(fileName)) {
            table = toml::parse_file(fileName.c_str());
        }
    }

    void Manifest::saveFile(fs::path fileName) {
        std::ofstream os{fileName};
        os << *this;
    }

    std::ostream &operator<<(std::ostream &os, const Manifest &manifest) {
        os << manifest.table;
        return os;
    }
}
