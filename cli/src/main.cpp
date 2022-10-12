#include <iostream>
#include <string>

#include "cli/options.h"
#include "config/config.h"
#include "repository/RemoteRepository.h"

using namespace oki;

int main(int argc, char *argv[]) {
    bool color = acceptColor();
    CliAction action = parseArguments(argc, argv);
    RemoteRepository repository{"http://localhost:8000"};
    if (std::holds_alternative<ListAction>(action)) {
        for (const Package &package : repository.listPackages()) {
            std::cout << package.getShortName() << ": " << package.getLongName() << "\n";
        }
    } else if (auto* install = std::get_if<InstallAction>(&action)) {
        std::cout << "Installing " << install->packageName << "...\n";
    } else if (auto* show = std::get_if<ShowAction>(&action)){
        std::optional<Package> p = repository.showPackage(show->packageName);
        if(p == std::nullopt){
            std::cout << "Le package n'existe pas\n";
        } else {
            if (color) {
                std::cout << "\e[32m" << p->getShortName() << "\e[0m";
            } else {
                std::cout << p->getShortName();
            }
            if (!p->getVersions().empty()) {
                const Version& latest = p->getVersions().front();
                std::cout << "/" << latest.getIdentifier() << " (" << latest.getPublishedDate() << ")";
            }
            std::cout << "\n\t" << p->getLongName() << "\n\n";
            for (const Version& version : p->getVersions()) {
                std::cout << "\t" << version.getIdentifier() << " (" << version.getPublishedDate() << ")\n";
            }
        }
    }
    return 0;
}
