#include "TreeAction.h"
#include "../config/ManifestLock.h"
#include "../io/oki.h"
#include "../tree/TreeRenderer.h"
#include <iostream>

namespace cli {
    void TreeAction::run(repository::Repository &repository) {
        config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
        config::ManifestLock manifestLock = config::ManifestLock::readOrResolve(OKI_MANIFEST_FILE, OKI_LOCK_FILE, repository);

        std::vector<std::string> directDependencies = manifest.listDeclaredPackagesNames();
        tree::TreeRenderer renderer{manifestLock, directDependencies};
        std::cout << renderer;
    }
}
