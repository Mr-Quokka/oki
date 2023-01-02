#include "Manifest.h"

#include "../semver/ParseException.h"
#include <toml.hpp>

namespace fs = std::filesystem;

constexpr static std::string_view PACKAGE_SECTION_NAME = "package";
constexpr static std::string_view DEPENDENCY_SECTION_NAME = "dependencies";

namespace config {
    Manifest Manifest::fromFile(const fs::path &fileName) {
        Manifest manifest;
        if (manifest.loadFileIfExists(fileName)) {
            return manifest;
        } else {
            throw ManifestException("This isn't an project using oki");
        }
    }

    std::unordered_map<std::string, semver::Range> Manifest::listDeclaredPackages() const {
        const auto *dependenciesTable = table.get_as<toml::table>(DEPENDENCY_SECTION_NAME);
        std::unordered_map<std::string, semver::Range> packages;
        if (dependenciesTable == nullptr) {
            return packages;
        }
        for (const auto &[dependency, constraint] : *dependenciesTable) {
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

    std::vector<std::string> Manifest::listDeclaredPackagesNames() const {
        const auto *dependenciesTable = table.get_as<toml::table>(DEPENDENCY_SECTION_NAME);
        std::vector<std::string> packages;
        if (dependenciesTable == nullptr) {
            return packages;
        }
        std::transform(
            dependenciesTable->cbegin(),
            dependenciesTable->cend(),
            std::back_inserter(packages),
            [](const auto &pair) {
                return std::string{pair.first};
            });
        return packages;
    }

    std::vector<fs::path> Manifest::getInclude() const {
        const auto *include = getPackageSection().get_as<toml::array>("include");
        std::vector<fs::path> paths;
        if (include == nullptr) {
            return paths;
        }
        paths.reserve(include->size());
        std::transform(
            include->cbegin(),
            include->cend(),
            std::back_inserter(paths),
            [](const auto &value) {
                return value.as_string()->get();
            });
        return paths;
    }

    bool Manifest::addDeclaredPackage(std::string_view packageName, std::string_view version) {
        return addDependencySectionIfNotExists().insert_or_assign(packageName, version).second;
    }

    bool Manifest::addDeclaredPackage(std::string_view packageName, semver::Version &version) {
        return addDeclaredPackage(packageName, version.str());
    }

    bool Manifest::removeDeclaredPackage(std::string_view packageName) {
        return addDependencySectionIfNotExists().erase(packageName) != 0;
    }

    toml::table &Manifest::addDependencySectionIfNotExists() {
        auto it = table.find(DEPENDENCY_SECTION_NAME);
        if (it == table.end()) {
            table.insert(DEPENDENCY_SECTION_NAME, toml::v3::table{});
        }
        return *table[DEPENDENCY_SECTION_NAME].as_table();
    }

    const toml::table &Manifest::getPackageSection() const {
        const toml::table *package = table.get_as<toml::table>(PACKAGE_SECTION_NAME);
        if (package == nullptr) {
            throw ManifestException{"Manifest is missing a [package] section"};
        }
        return *package;
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
