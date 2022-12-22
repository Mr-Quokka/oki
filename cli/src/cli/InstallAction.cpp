#include "InstallAction.h"
#include "../config/ManifestLock.h"
#include "../io/HttpRequest.h"
#include "../io/oki.h"
#include "../op/fetch.h"
#include <iostream>

namespace cli {
    InstallAction::InstallAction(const char *packageName) : packageName{packageName} {}

    void InstallAction::run(repository::Repository &repository) {
        package::Package p = repository.getPackageInfo(packageName);
        if (p.getVersions().empty()) {
            throw io::APIException{"The package doesn't have any version"};
        } else {
            config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
            package::PackageVersion latest = p.getVersions().front();
            if (manifest.addDeclaredPackage(packageName, latest)) {
                std::cout << "Adding " << packageName << " v" << latest << " to dependencies.\n\n";
            }
            manifest.saveFile(OKI_MANIFEST_FILE);

            solver::Resolved resolved = solver::resolve(manifest.listDeclaredPackages(), repository);
            config::ManifestLock lock{resolved};
            op::fetch(resolved, std::cout, {std::string{packageName}});
            lock.saveFile(OKI_LOCK_FILE);
        }
    }
}
