#include "ManifestLock.h"

#include <iostream>
#include <toml.hpp>
#include <utility>

#include "../solver/Resolver.h"
#include "Manifest.h"

namespace fs = std::filesystem;

namespace config {
    ManifestLock::ManifestLock(Packages resolved)
        : locks{std::move(resolved)} {}

    ManifestLock::ManifestLock(const solver::Resolved &resolved) {
        for (const auto &[package, version] : resolved) {
            std::vector<std::string> dependencies;
            std::transform(
                version.getDependencies().cbegin(),
                version.getDependencies().cend(),
                std::back_inserter(dependencies),
                [](const auto &dep) {
                    return dep.first;
                });
            locks.insert({package, {version, version.getDownloadUrl(), std::move(dependencies)}});
        }
    }

    const ManifestLock::Packages &ManifestLock::getLocks() const {
        return locks;
    }

    ManifestLock::Packages::const_iterator ManifestLock::find(const std::string &packageName) const {
        return locks.find(packageName);
    }

    ManifestLock::Packages::const_iterator ManifestLock::cbegin() const {
        return locks.cbegin();
    }

    ManifestLock::Packages::const_iterator ManifestLock::cend() const {
        return locks.cend();
    }

    ManifestLock ManifestLock::fromFile(const fs::path &fileName) {
        toml::table toml = toml::parse_file(fileName.string());
        std::unordered_map<std::string, package::VersionLock> locks;
        for (const auto &[package, value] : toml) {
            toml::table *properties = value.as_table();
            package::VersionLock::DepsType deps;
            const toml::node *dependencies = properties->get("dependencies");
            if (dependencies != nullptr) {
                dependencies->visit([&](const toml::array &array) {
                    std::transform(
                        array.cbegin(),
                        array.cend(),
                        std::back_inserter(deps),
                        [](const auto &value) {
                            return value.as_string()->get();
                        });
                });
            }
            locks.insert(std::make_pair(
                package,
                package::VersionLock{
                    semver::Version::parse(properties->get("version")->as_string()->get()),
                    properties->get("source")->as_string()->get(), deps}));
        }
        return ManifestLock{locks};
    }

    ManifestLock ManifestLock::readOrResolve(const fs::path &manifestFileName, const fs::path &lockFileName, repository::Repository &repository) {
        if (fs::exists(lockFileName)) {
            return fromFile(lockFileName);
        }
        Manifest manifest = Manifest::fromFile(manifestFileName);
        ManifestLock lock = ManifestLock{solver::resolve(manifest.listDeclaredPackages(), repository)};
        lock.saveFile(lockFileName);
        return lock;
    }

    void ManifestLock::saveFile(const fs::path &fileName) const {
        std::ofstream os{fileName};
        os << *this;
    }

    std::ostream &operator<<(std::ostream &os, const ManifestLock &lock) {
        toml::table table;
        for (const auto &[package, version] : lock.getLocks()) {
            toml::table versionTable{
                {"version", version.str()},
                {"source", version.getDownloadUrl()}};
            if (!version.getDependencies().empty()) {
                toml::array dependencies;
                for (const std::string &dependency : version.getDependencies()) {
                    dependencies.push_back(dependency);
                }
                table.insert("dependencies", dependencies);
            }
            table.insert(package, versionTable);
        }
        return os << table;
    }
}
