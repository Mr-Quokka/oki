#include "ShowAction.h"
#include "../config/config.h"

#include "ExitStatuses.h"
#include <iostream>

namespace cli {
    ShowAction::ShowAction(config::UserConfig &config, ArgMatches &&args)
        : packageName{args.require<std::string>("package")}, repository{args.getRegistry(config)} {}

    int ShowAction::run() {
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
        return OK;
    }

    Command ShowAction::cmd() {
        return Command{"show", "Show package information", [](config::UserConfig &config, ArgMatches &&args) -> std::unique_ptr<CliAction> {
                           return std::make_unique<ShowAction>(config, std::move(args));
                       }}
            .arg<std::string>("registry", "Name of the registry to use")
            .arg<std::string>("package", "Name of the package to show")
            .positional("package");
    }
}
