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
            std::cout << "\x1B[32m" << p.getName() << "\x1B[0m";
        } else {
            std::cout << p.getName();
        }
        if (!p.getVersions().empty()) {
            const package::PackageVersion &latest = p.getVersions().front();
            std::cout << "/" << latest << " (" << latest.getPublishedDate() << ")";
            if (!latest.getDependencies().empty()) {
                std::cout << "\ndependencies : ";
                for (auto it = latest.getDependencies().begin(); it != latest.getDependencies().end(); it++) {
                    if (it == latest.getDependencies().begin()) {
                        std::cout << it->first;
                    } else {
                        std::cout << ", " << it->first;
                    }
                }
            }
        }
        std::cout << "\n\t" << p.getDescription() << "\n\n";
        for (const package::PackageVersion &version : p.getVersions()) {
            std::cout << "\t" << version << " (" << version.getPublishedDate() << ")\n";
        }
    }
}
