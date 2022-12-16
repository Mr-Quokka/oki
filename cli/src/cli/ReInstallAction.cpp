#include "ReInstallAction.h"
#include "../config/ManifestLock.h"
#include "../io/oki.h"
#include "../solver/Resolver.h"

#include <iostream>

namespace fs = std::filesystem;

namespace cli {
    void ReInstallAction::run(repository::Repository &repository) {
        config::ManifestLock manifest = config::ManifestLock::readOrResolve(OKI_MANIFEST_FILE, OKI_LOCK_FILE, repository);
        std::cout << "Reinstalling all packages\n";
        for (const auto &[dependency, version] : manifest.getLocks()) {
            fs::create_directories(OKI_PACKAGES_DIRECTORY / dependency);
            repository.download(version, OKI_PACKAGES_DIRECTORY / dependency);
        }
    }
}
