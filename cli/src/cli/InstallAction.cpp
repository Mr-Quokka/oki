#include "InstallAction.h"
#include "../config/Manifest.h"
#include "../io/HttpRequest.h"
#include "../io/oki.h"
#include <iostream>

namespace fs = std::filesystem;

namespace cli {
    InstallAction::InstallAction(const char *packageName) : packageName{packageName} {}

    void InstallAction::run(repository::Repository &repository) {
        package::Package p = repository.getPackageInfo(packageName);
        if (p.getVersions().empty()) {
            throw io::APIException{"The packet doesn't have any version"};
        } else {
            config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
            package::Version latest = p.getVersions().front();
            if (manifest.addDeclaredPackage(packageName, latest.getIdentifier())) {
                std::cout << "Adding " << packageName << " v" << latest.getIdentifier() << " to dependencies.\n";
            }
            manifest.saveFile(OKI_MANIFEST_FILE);

            fs::create_directories(OKI_PACKAGES_DIRECTORY);
            repository.download(latest, OKI_PACKAGES_DIRECTORY);
        }
    }
}
