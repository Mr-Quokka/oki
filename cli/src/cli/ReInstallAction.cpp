#include "ReInstallAction.h"
#include "../config/Manifest.h"
#include "../io/oki.h"

#include <iostream>

namespace fs = std::filesystem;

namespace oki {
    ReInstallAction::ReInstallAction() {}

    void ReInstallAction::run(Repository &repository) {
        std::cout << "Reinstalling all the packages in the manifest ...\nPackage -> Version\n";
        Manifest manifest = Manifest::fromFile(OKI_MANIFEST_FILE);
        fs::create_directories(OKI_PACKAGES_DIRECTORY);
        std::cout << "Reinstalling all the packages in the manifest ...\nPackage -> Version\n";
        for (const std::pair<std::string_view, std::string> package : manifest.listDeclaredPackages()) {
            std::cout << package.first << " -> " << package.second << "\n";
            repository.download(Version{package.second, "", repository.getPackageURL(package.first, package.second)}, OKI_PACKAGES_DIRECTORY);
        }
    }
}