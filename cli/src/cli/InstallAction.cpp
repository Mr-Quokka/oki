#include "InstallAction.h"
#include "../config/Manifest.h"
#include "../io/HttpRequest.h"
#include "../io/oki.h"

namespace fs = std::filesystem;

namespace oki {
    InstallAction::InstallAction(const char *packageName) : packageName{packageName} {}

    void InstallAction::run(Repository &repository) {
        Package p = repository.getPackageInfo(packageName);
        if (p.getVersions().empty()) {
            throw APIException{"The packet doesn't have any version"};
        } else {
            Manifest manifest = Manifest::fromFile(OKI_MANIFEST_FILE);
            manifest.addDeclaredPackage(packageName, p.getVersions().front().getIdentifier());
            manifest.saveFile(OKI_MANIFEST_FILE);

            fs::create_directories(OKI_PACKAGES_DIRECTORY);
            repository.download(p.getVersions().front(), OKI_PACKAGES_DIRECTORY);
        }
    }
}
