#include "ReInstallAction.h"
#include "../config/Manifest.h"
#include "../io/oki.h"

#include <iostream>

namespace fs = std::filesystem;

namespace cli {
    void ReInstallAction::run(repository::Repository &repository) {
        std::cout << "Reinstalling all packages\n";
        config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
        fs::create_directories(OKI_PACKAGES_DIRECTORY);
        for (const std::pair<std::string_view, std::string> package : manifest.listDeclaredPackages()) {
            std::cout << package.first << " -> " << package.second << "\n";
            repository.download(package::PackageVersion{semver::Version{}, "", repository.getPackageURL(package.first, package.second)}, OKI_PACKAGES_DIRECTORY);
        }
    }
}
