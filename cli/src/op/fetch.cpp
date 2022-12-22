#include "fetch.h"
#include "io/Installer.h"
#include "io/oki.h"

namespace op {
    template <DownloadableVersionConcept T>
    int fetch(const std::unordered_map<std::string, T> &resolved, std::ostream &out, const LogOptions &options, const std::filesystem::path &workingDirectory) {
        io::Installer installer{config::InstallationRegistry::loadFileIfExists(workingDirectory / OKI_INTERNAL_REGISTRY_FILE), workingDirectory / OKI_PACKAGES_DIRECTORY};

        unsigned int installed = 0;
        unsigned int updated = 0;
        for (auto &[package, versionRef] : resolved) {
            package::DownloadableVersion version = versionRef;
            auto [it, result] = installer.install(package, version);
            if (result == io::InstallationResult::NoChange) {
                continue;
            }
            if (result == io::InstallationResult::Installed) {
                ++installed;
            } else {
                ++updated;
            }
            if (std::find(options.logWhenSeen.cbegin(), options.logWhenSeen.cend(), package) != options.logWhenSeen.cend()) {
                if (result == io::InstallationResult::Updated) {
                    out << " - " << package << " " << version << "\n";
                }
                out << " + " << package << " " << version << "\n";
            }
        }

        if (installed != 0) {
            out << "Installed " << installed << " package";
            if (installed > 1) {
                out << "s";
            }
            out << "\n";
        }
        if (updated != 0) {
            out << "Updated " << updated << " package";
            if (updated > 1) {
                out << "s";
            }
            out << "\n";
        }
        if (installed == 0 && updated == 0 && options.logWhenUpToDate) {
            out << "Already up-to-date\n";
        }

        installer.saveRegistry(OKI_INTERNAL_REGISTRY_FILE);
        return 0;
    }

    template int fetch<package::VersionLock>(
        const std::unordered_map<std::string, package::VersionLock> &,
        std::ostream &,
        const LogOptions &,
        const std::filesystem::path &workingDirectory);
    template int fetch<package::PackageVersion>(
        const std::unordered_map<std::string, package::PackageVersion> &,
        std::ostream &,
        const LogOptions &,
        const std::filesystem::path &workingDirectory);
}
