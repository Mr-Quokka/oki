#include <iostream>
#include <string>

#include "cli/options.h"
#include "config/Manifest.h"
#include "config/config.h"
#include "io/HttpRequest.h"
#include "repository/RemoteRepository.h"

namespace fs = std::filesystem;

using namespace oki;

int main(int argc, char *argv[]) {
    bool color = acceptColor();
    CliAction action = parseArguments(argc, argv);
    RemoteRepository repository{"http://localhost:8000"};
    if (std::holds_alternative<ListAction>(action)) {
        for (const Package &package : repository.listPackages()) {
            std::cout << package.getShortName() << ": " << package.getLongName() << "\n";
        }
    } else if (auto *install = std::get_if<InstallAction>(&action)) {
        std::optional<Package> p = repository.showPackage(install->packageName);
        if (p->getVersions().empty()) {
            throw APIException{"The packet doesn't have any version"};
        } else if (p == std::nullopt) {
            throw APIException("This packet doesn't exist");
        } else {
            Manifest manifest{"oki.toml"};
            manifest.addDeclaredPackage(install->packageName, p->getVersions().front().getIdentifier());
            std::ofstream os{"oki.toml"};
            os << manifest;

            fs::path packagesPath{"oki-packages"};
            fs::create_directories(packagesPath);
            repository.download(p->getVersions().front(), packagesPath);
        }
    } else if (auto *show = std::get_if<ShowAction>(&action)) {
        std::optional<Package> p = repository.showPackage(show->packageName);
        if (p == std::nullopt) {
            std::cerr << "This packet doesn't exist\n";
        } else {
            if (color) {
                std::cout << "\x1B[32m" << p->getShortName() << "\x1B[0m";
            } else {
                std::cout << p->getShortName();
            }
            if (!p->getVersions().empty()) {
                const Version &latest = p->getVersions().front();
                std::cout << "/" << latest.getIdentifier() << " (" << latest.getPublishedDate() << ")";
            }
            std::cout << "\n\t" << p->getLongName() << "\n\n";
            for (const Version &version : p->getVersions()) {
                std::cout << "\t" << version.getIdentifier() << " (" << version.getPublishedDate() << ")\n";
            }
        }
    }
    return 0;
}
