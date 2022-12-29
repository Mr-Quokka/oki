#include "RemoveAction.h"
#include "../config/ManifestLock.h"
#include "../io/oki.h"
#include "../op/fetch.h"

#include <iostream>

namespace cli {
    RemoveAction::RemoveAction(const char *packageName) : packageName{packageName} {}

    void RemoveAction::run(repository::Repository &repository) {
        config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
        if (!manifest.removeDeclaredPackage(packageName)) {
            std::cerr << "The dependency `" << packageName << "` could not be found in `dependencies`.\n";
            exit(1);
        }
        std::cout << "Removing " << packageName << " from dependencies\n";
        manifest.saveFile(OKI_MANIFEST_FILE);
        config::ManifestLock manifestLock = config::ManifestLock::readOrResolve(OKI_MANIFEST_FILE, OKI_LOCK_FILE, repository);
        std::vector<std::string> directDependencies = manifest.listDeclaredPackagesNames();
        directDependencies.emplace_back(packageName);
        op::fetch(manifestLock, std::cout, {directDependencies, false});
        manifestLock.saveFile(OKI_LOCK_FILE);
    }
}
