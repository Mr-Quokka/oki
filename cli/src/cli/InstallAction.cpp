#include "InstallAction.h"
#include "../config/Manifest.h"
#include "../io/HttpRequest.h"


#include <iostream>

namespace fs = std::filesystem;

namespace oki {
	InstallAction::InstallAction(const char *packageName) : packageName{packageName} {}


	void InstallAction::run(Repository& repository) {
        std::optional<Package> p = repository.showPackage(packageName);
        if (p->getVersions().empty()) {
            throw APIException{"The packet doesn't have any version"};
        } else if (p == std::nullopt) {
            throw APIException("This packet doesn't exist");
        } else {
            Manifest manifest{"oki.toml"};
            manifest.addDeclaredPackage(packageName, p->getVersions().front().getIdentifier());
            std::ofstream os{"oki.toml"};
            os << manifest;

            fs::path packagesPath{"oki-packages"};
            fs::create_directories(packagesPath);
            repository.download(p->getVersions().front(), packagesPath);
	   }
    }
}