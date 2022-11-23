#include "ShowAction.h"
#include "../config/config.h"

#include <iostream>

namespace fs = std::filesystem;

namespace cli {
    ShowAction::ShowAction(const char *packageName) : packageName{packageName} {}

    void ShowAction::run(repository::Repository &repository) {
        bool color = config::acceptColor();
        package::Package p = repository.getPackageInfo(packageName);
        if (color) {
            std::cout << "\x1B[32m" << p.getShortName() << "\x1B[0m";
        } else {
            std::cout << p.getShortName();
        }
        if (!p.getVersions().empty()) {
            const package::PackageVersion &latest = p.getVersions().front();
            std::cout << "/" << latest << " (" << latest.getPublishedDate() << ")";
        }
        std::cout << "\n\t" << p.getDescription() << "\n\n";
        for (const package::PackageVersion &version : p.getVersions()) {
            std::cout << "\t" << version << " (" << version.getPublishedDate() << ")\n";
        }
    }
}
