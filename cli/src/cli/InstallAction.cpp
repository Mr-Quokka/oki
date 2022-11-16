#include "InstallAction.h"
#include "../config/Manifest.h"
#include "../io/HttpRequest.h"
#include "../io/oki.h"

namespace fs = std::filesystem;

namespace cli {
    InstallAction::InstallAction(const char *packageName) : packageName{packageName} {}

    void InstallAction::run(repository::Repository &repository) {
        std::optional<package::Package> p = repository.showPackage(packageName);
        if (p->getVersions().empty()) {
            throw io::APIException{"The packet doesn't have any version"};
        } else if (p == std::nullopt) {
            throw io::APIException("This packet doesn't exist");
        } else {
            config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
            manifest.addDeclaredPackage(packageName, p->getVersions().front().getIdentifier());
            manifest.saveFile(OKI_MANIFEST_FILE);

            fs::create_directories(OKI_PACKAGES_DIRECTORY);
            repository.download(p->getVersions().front(), OKI_PACKAGES_DIRECTORY);
        }
    }
}