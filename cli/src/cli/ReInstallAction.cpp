#include "ReInstallAction.h"
#include "../config/Manifest.h"
#include "../io/oki.h"
#include "../solver/Resolver.h"

#include <iostream>

namespace fs = std::filesystem;

namespace cli {
    void ReInstallAction::run(repository::Repository &repository) {
        std::cout << "Reinstalling all packages\n";
        config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
        fs::create_directories(OKI_PACKAGES_DIRECTORY);
        solver::Resolved resolved = solver::resolve(manifest.listDeclaredPackages(), repository);
        for (auto const &[dependency, version] : resolved) {
            fs::create_directories(OKI_PACKAGES_DIRECTORY/dependency);
            repository.download(version, OKI_PACKAGES_DIRECTORY/dependency);
        }
    }
}
