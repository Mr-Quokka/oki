#include "fetch.h"
#include "io/Installer.h"
#include "io/oki.h"

namespace op {
    int fetch(const config::ManifestLock &manifestLock, std::ostream &out, const LogOptions &options, const std::filesystem::path &workingDirectory) {
        std::filesystem::path packagesDir = workingDirectory / OKI_PACKAGES_DIRECTORY;
        io::Installer installer{config::InstallationRegistry::loadFileIfExists(workingDirectory / OKI_INTERNAL_REGISTRY_FILE), packagesDir};

        unsigned int installed = 0;
        unsigned int updated = 0;
        for (const auto &[package, versionRef] : manifestLock.getLocks()) {
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
                out << " + " << package << " " << versionRef << "\n";
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

        make::BuildConfigurer config{solver::DependencyGraph{manifestLock}, packagesDir};
        installer.configure(config);

        unsigned int removed = installer.uninstallUnreachable([&](const std::string &package, const package::DownloadableVersion &version) {
            bool reachable = manifestLock.contains(package);
            if (reachable) {
                return true;
            }
            if (std::find(options.logWhenSeen.cbegin(), options.logWhenSeen.cend(), package) != options.logWhenSeen.cend()) {
                out << " - " << package << " " << version << "\n";
            }
            return false;
        });
        if (removed != 0) {
            out << "Removed " << removed << " package";
            if (removed > 1) {
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

    int fetch(const config::ManifestLock &manifestLock, std::ostream &out, const LogOptions &options) {
        return fetch(manifestLock, out, options, std::filesystem::current_path());
    }
}
