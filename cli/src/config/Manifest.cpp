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
            throw ManifestException("This isn't a project using oki");
        }
    }

    std::unordered_map<std::string, package::VersionRequirement> Manifest::listDeclaredPackages() const {
        const auto *dependenciesTable = table.get_as<toml::table>(DEPENDENCY_SECTION_NAME);
        std::unordered_map<std::string, package::VersionRequirement> packages;
        if (dependenciesTable == nullptr) {
            return packages;
        }
        for (const auto &[dependency, constraint] : *dependenciesTable) {
            std::string constraintValue;
            std::optional<std::string> registry;
            const auto *dependencyString = constraint.as_string();
            const auto *dependencyTable = constraint.as_table();
            if (dependencyTable != nullptr) {
                const auto *versionField = dependencyTable->get_as<std::string>("constraintValue");
                if (versionField != nullptr) {
                    constraintValue = versionField->get();
                }
                registry = (*dependencyTable)["registry"].value<std::string>();
            } else if (dependencyString != nullptr) {
                constraintValue = dependencyString->get();
            } else {
                throw ManifestException("Dependency `" + std::string{dependency} + "` has an invalid format");
            }

            if (constraintValue.empty()) {
                throw ManifestException("Dependency `" + std::string{dependency} + "` is missing a constraintValue field");
            }
            try {
                packages.emplace(dependency, package::VersionRequirement{semver::Range::parse(constraintValue), std::move(registry)});
            } catch (semver::ParseException &ex) {
                ex.addContext("Failed to parse the version requirement `" + constraintValue + "` for dependency `" + std::string{dependency} + "`");
                throw;
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

    std::string Manifest::getProjectName() const {
        const auto *name = getPackageSection().get_as<std::string>("name");
        if (name == nullptr) {
            throw ManifestException{"Manifest is missing a [package].name field"};
        }
        return name->get();
    }

    std::optional<std::string> Manifest::getDescription() const {
        return getPackageSection()["description"].value<std::string>();
    }

    fs::path Manifest::getPackageArchive() const {
        fs::path packageArchive{getProjectName()};
        packageArchive += "_";
        packageArchive += getPackageVersion();
        packageArchive += ".zip";
        return packageArchive;
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

    make::ProjectKind Manifest::getProjectKind() const {
        const auto *kind = getPackageSection().get_as<std::string>("kind");
        if (kind == nullptr) {
            throw ManifestException{"Manifest is missing a [package].kind field"};
        }
        return make::projectKindFromStr(kind->get());
    }

    make::BuildInfo Manifest::getBuildInfo() const {
        auto view = table["lib"]["c"];
        auto exportOpt = view["export"].value<std::string>();
        return make::BuildInfo{
            view["build-static"].value<std::string>(),
            exportOpt.has_value() ? std::vector<std::string>{exportOpt.value()} : std::vector<std::string>{},
            view["static-link"].value<std::string>()};
    }

    bool Manifest::addDeclaredPackage(std::string_view packageName, std::string_view version) {
        return addDependencySectionIfNotExists().insert_or_assign(packageName, version).second;
    }

    bool Manifest::addDeclaredPackage(std::string_view packageName, package::VersionRequirement &&requirement) {
        if (requirement.getRegistry().has_value()) {
            return addDependencySectionIfNotExists().insert_or_assign(
                                                        packageName,
                                                        toml::table{
                                                            {"constraintValue", requirement.getRange().str()},
                                                            {"registry", requirement.getRegistry().value()}})
                .second;
        }
        return addDeclaredPackage(packageName, requirement.getRange().str());
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

    semver::Version Manifest::getPackageVersion() const {
        const auto *version = getPackageSection().get_as<std::string>("version");
        if (version == nullptr) {
            throw ManifestException{"Manifest is missing a [package].version field"};
        }
        return semver::Version::parse(version->get());
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
