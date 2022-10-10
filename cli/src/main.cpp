#include <iostream>
#include <string>

#include "cli/options.h"
#include "repository/RemoteRepository.h"

using namespace oki;

std::size_t writeCallback(char *in, size_t size, size_t nmemb, std::string *out){ 
    std::size_t total_size = size * nmemb;
    if (total_size){
        out->append(in, total_size);
        return total_size;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    CliAction action = parseArguments(argc, argv);
    RemoteRepository repository{"http://localhost:8000"};
    if (std::holds_alternative<ListAction>(action)) {
        for (const Package &package : repository.listPackages()) {
            std::cout << package.getShortName() << ": " << package.getLongName() << "\n";
        }
    } else if (auto* install = std::get_if<InstallAction>(&action)) {
        std::cout << "Installing " << install->packageName << "...\n";
    } else if(auto* show = std::get_if<ShowAction>(&action)){
        std::optional<Package> p = repository.showPackage(show->packageName);
        if(p == std::nullopt){
            std::cout << "Le package n'existe pas\n";
        }
        else std::cout << p->getShortName() << "/" << p->getVersions().front().getIdentifier() << "\t"
            << p->getVersions().front().getPublishedDate()<< "\n\t" << p->getLongName() << "\n";
    }
    return 0;
}
