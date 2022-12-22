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
            fs::remove_all(packageName);
        }

        fs::create_directories(dependencyPath);
        io::HttpRequest request = HttpRequest::createJson(version.getDownloadUrl());
        io::TmpFile tmp;
        request.download(tmp.getFilename());
        io::Extractor extractor{dependencyPath};
        extractor.extract(tmp.getFilename());
        return std::make_pair(
            it,
            inserted ? InstallationResult::Installed : InstallationResult::Updated);
    }

    bool Installer::uninstall(const std::string &packageName) {
        if (registry.erase(packageName)) {
            fs::remove_all(getDependencyPath(packageName));
            return true;
        }
        return false;
    }

    void Installer::uninstallUnreachable(std::function<bool(const std::string &)> &&isReachable) {
        auto it = registry.begin();
        while (it != registry.end()) {
            if (!isReachable(it->first)) {
                fs::remove_all(getDependencyPath(it->first));
                it = registry.erase(it);
            } else {
                ++it;
            }
        }
    }

    fs::path Installer::getDependencyPath(std::string_view dependencyName) {
        return workingDirectory / dependencyName;
    }

    void Installer::saveRegistry(const fs::path &path) {
        registry.saveToFile(path);
    }
}