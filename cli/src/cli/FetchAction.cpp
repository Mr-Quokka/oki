#include "FetchAction.h"
#include "../config/ManifestLock.h"
#include "../io/oki.h"
#include "../op/fetch.h"

#include <iostream>

namespace cli {
    void FetchAction::run(repository::Repository &repository) {
        config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
        config::ManifestLock manifestLock = config::ManifestLock::readOrResolve(OKI_MANIFEST_FILE, OKI_LOCK_FILE, repository);
        op::fetch(manifestLock.getLocks(), std::cout, {manifest.listDeclaredPackagesNames(), true});
    }
}
