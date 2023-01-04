#include "Installer.h"
#include "Archive.h"
#include "HttpRequest.h"
#include "TmpFile.h"

namespace fs = std::filesystem;

namespace io {
    Installer::Installer(config::InstallationRegistry &&registry, const fs::path &workingDirectory)
        : registry{registry}, workingDirectory{workingDirectory} {}

    std::pair<Installer::Current, InstallationResult> Installer::install(const std::string &packageName, package::DownloadableVersion &version) {
        fs::path dependencyPath = getDependencyPath(packageName);

        // Insère la nouvelle dépendance ou récupère la valeur de ce qui est déjà installé
        auto [it, inserted] = registry.emplace(packageName, version);
        if (!inserted) {
            // Si exactement cette version est installée, alors il n'y a rien à faire
            if (it->second == version) {
                return std::make_pair(it, InstallationResult::NoChange);
            }

            // Sinon, remplacer l'ancienne
            std::swap(it->second, version);
            fs::remove_all(dependencyPath);
        }

        fs::create_directories(dependencyPath);
        io::HttpRequest request = HttpRequest::createJson(version.getDownloadUrl());
        io::TmpFile tmp;
        request.download(tmp.path());
        io::Extractor extractor{dependencyPath};
        extractor.extract(tmp.path());
        pendingConfigurations.emplace_back(packageName);
        return std::make_pair(
            it,
            inserted ? InstallationResult::Installed : InstallationResult::Updated);
    }

    void Installer::configure(make::BuildConfigurer &configurer) {
        for (const std::string &package : pendingConfigurations) {
            configurer.configure(package);
        }
        pendingConfigurations.clear();
    }

    bool Installer::uninstall(const std::string &packageName) {
        if (registry.erase(packageName)) {
            fs::remove_all(getDependencyPath(packageName));
            return true;
        }
        return false;
    }

    fs::path Installer::getDependencyPath(std::string_view dependencyName) {
        return workingDirectory / dependencyName;
    }

    void Installer::saveRegistry(const fs::path &path) {
        registry.saveToFile(path);
    }
}
