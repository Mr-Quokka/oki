#include "InstallAction.h"
#include "../config/Manifest.h"
#include "../io/HttpRequest.h"
#include "../io/oki.h"

#include <iostream>

namespace fs = std::filesystem;

namespace oki {
    InstallAction::InstallAction(const char *packageName) : packageName{packageName} {}

    void InstallAction::run(Repository &repository) {
        std::optional<Package> p = repository.showPackage(packageName);
        if (p->getVersions().empty()) {
            throw APIException{"The packet doesn't have any version"};
        } else if (p == std::nullopt) {
            throw APIException("This packet doesn't exist");
        } else {
            Manifest manifest = Manifest::fromFile(OKI_MANIFEST_FILE);
            manifest.addDeclaredPackage(packageName, p->getVersions().front().getIdentifier());
            manifest.saveIntoFile(OKI_MANIFEST_FILE);

            fs::create_directories(OKI_PACKAGES_DIRECTORY);
            repository.download(p->getVersions().front(), OKI_PACKAGES_DIRECTORY);
        }
    }
}