#include "ShowAction.h"
#include "../config/config.h"

#include <iostream>

namespace fs = std::filesystem;

namespace cli {
    ShowAction::ShowAction(const char *packageName) : packageName{packageName} {}

    void ShowAction::run(repository::Repository &repository) {
        bool color = config::acceptColor();
        std::optional<package::Package> p = repository.showPackage(packageName);
        if (p == std::nullopt) {
            std::cerr << "This packet doesn't exist\n";
        } else {
            if (color) {
                std::cout << "\x1B[32m" << p->getShortName() << "\x1B[0m";
            } else {
                std::cout << p->getShortName();
            }
            if (!p->getVersions().empty()) {
                const package::Version &latest = p->getVersions().front();
                std::cout << "/" << latest.getIdentifier() << " (" << latest.getPublishedDate() << ")";
            }
            std::cout << "\n\t" << p->getShortName() << "\n\n";
            for (const package::Version &version : p->getVersions()) {
                std::cout << "\t" << version.getIdentifier() << " (" << version.getPublishedDate() << ")\n";
            }
        }
    }
}
