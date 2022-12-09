#include "Manifest.h"

#include "../semver/ParseException.h"
#include <toml.hpp>

namespace fs = std::filesystem;

constexpr static std::string_view PACKAGE_SECTION_NAME = "package";
constexpr static std::string_view DEPENDENCY_SECTION_NAME = "dependencies";

namespace config {
    Manifest Manifest::fromFile(const fs::path &fileName) {
        Manifest manifest;
        if (manifest.loadFileIfExists(fileName) == true) {
            return manifest;
        } else {
            throw ManifestException("This isn't an project using oki");
        }
    }

    std::unordered_map<std::string, semver::Range> Manifest::listDeclaredPackages() const {
        std::unordered_map<std::string, semver::Range> packages;
        for (const auto &[dependency, constraint] : *table[DEPENDENCY_SECTION_NAME].as_table()) {
            std::string constraintValue = constraint.as_string()->get();
            try {
                packages.emplace(dependency, semver::Range::parse(constraintValue));
            } catch (semver::ParseException &ex) {
                ex.addContext("Failed to parse the version requirement `" + constraintValue + "` for dependency `" + std::string{dependency} + "`");
                throw ex;
            }
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

    bool Manifest::loadFileIfExists(const fs::path &fileName) {
        fs::path searchedFile = fs::absolute(fileName);
        fs::path parentPath = searchedFile;
        parentPath.remove_filename();
        while (parentPath != parentPath.parent_path()) {
            if (fs::exists(searchedFile)) {
                table = toml::parse_file(searchedFile.c_str());
                return true;
            } else {
                parentPath = parentPath.parent_path();
                searchedFile = parentPath / fileName.filename();
            }
        }
        return false;
    }

    void Manifest::saveFile(const fs::path &fileName) {
        std::ofstream os{fileName};
        os << *this;
    }

    std::string Manifest::asFilteredJson() const {
        std::stringstream stream;
        toml::table filtered;
        const toml::node *package = table.get(PACKAGE_SECTION_NAME);
        if (package != nullptr) {
            package->visit([&](const toml::table &t) {
                filtered.insert(t.cbegin(), t.cend());
            });
        }
        const toml::node *dependencies = table.get(DEPENDENCY_SECTION_NAME);
        if (dependencies != nullptr) {
            dependencies->visit([&](const toml::table &t) {
                filtered.insert(DEPENDENCY_SECTION_NAME, t);
            });
        }
        stream << toml::json_formatter{filtered};
        return stream.str();
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
