#include "Manifest.h"

#include <toml.hpp>

namespace fs = std::filesystem;

constexpr static std::string_view DEPENDENCY_SECTION_NAME = "dependencies";

namespace oki {
    Manifest::Manifest(fs::path fileName) {
        if (fs::exists(fileName)) {
            table = toml::parse_file(fileName.c_str());
        }
    }

    std::unordered_map<std::string_view, std::string> Manifest::listDeclaredPackages() const {
        std::unordered_map<std::string_view, std::string> packages;
        for (const auto &dependency : *table[DEPENDENCY_SECTION_NAME].as_table()) {
            packages.emplace(dependency.first, dependency.second.as_string()->get());
        }
        return packages;
    }

    void Manifest::addDeclaredPackage(std::string_view packageName, std::string_view version) {
        //auto& dependencies = *table[DEPENDENCY_SECTION_NAME].as_table();
        addDependencySectionIfNotExists().insert(packageName, version);
    }

    toml::v3::table &Manifest::addDependencySectionIfNotExists() {
        auto it = table.find(DEPENDENCY_SECTION_NAME);
        if (it == table.end()) {
            table.insert(DEPENDENCY_SECTION_NAME, toml::v3::table{});
        }
        return *table[DEPENDENCY_SECTION_NAME].as_table();
    }

    std::ostream &operator<<(std::ostream &os, const Manifest &manifest) {
        os << manifest.table;
        return os;
    }
}
