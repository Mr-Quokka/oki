#include "InstallationRegistry.h"

#include <toml.hpp>

namespace fs = std::filesystem;

namespace config {
    InstallationRegistry::Installed::const_iterator InstallationRegistry::cbegin() const {
        return alreadyInstalled.cbegin();
    }

    InstallationRegistry::Installed::const_iterator InstallationRegistry::cend() const {
        return alreadyInstalled.cend();
    }

    InstallationRegistry::Installed::const_iterator InstallationRegistry::find(const std::string &packageName) const {
        return alreadyInstalled.find(packageName);
    }

    bool InstallationRegistry::containsExact(const std::string &packageName, const package::DownloadableVersion &version) const {
        auto existing = alreadyInstalled.find(packageName);
        if (existing == alreadyInstalled.cend()) {
            return false;
        }
        return existing->second == version;
    }

    std::pair<InstallationRegistry::Installed::iterator, bool> InstallationRegistry::emplace(const std::string &packageName, const package::DownloadableVersion &version) {
        return alreadyInstalled.emplace(packageName, version);
    }

    bool InstallationRegistry::erase(const std::string &packageName) {
        return alreadyInstalled.erase(packageName) != 0;
    }

    InstallationRegistry InstallationRegistry::loadFileIfExists(const fs::path &path) {
        InstallationRegistry registry;
        if (!fs::exists(path)) {
            return registry;
        }

        toml::table read = toml::parse_file(path.string());
        for (const auto &[name, content] : read) {
            const auto *contentTable = content.as_table();
            if (contentTable == nullptr) {
                continue;
            }
            registry.alreadyInstalled.emplace(name, package::DownloadableVersion{
                                                        semver::Version::parse(contentTable->get_as<std::string>("version")->get()),
                                                        contentTable->get_as<std::string>("source")->get(),
                                                    });
        }
        return registry;
    }

    void InstallationRegistry::saveToFile(const fs::path &path) {
        toml::table table;
        for (const auto &[packageName, installedVersion] : alreadyInstalled) {
            toml::table version{
                {"version", installedVersion.str()},
                {"source", installedVersion.getDownloadUrl()},
            };
            table.insert(packageName, version);
        }
        std::ofstream os{path};
        os << table;
    }
}