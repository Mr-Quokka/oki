#include "RemoveAction.h"
#include "../config/ManifestLock.h"
#include "../io/oki.h"
#include "../op/fetch.h"
#include "ExitStatuses.h"

#include <iostream>

namespace cli {
    RemoveAction::RemoveAction(config::UserConfig &config, ArgMatches &&args)
        : config{config}, packageName{args.require<std::string>("package")} {}

    int RemoveAction::run() {
        config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
        if (!manifest.removeDeclaredPackage(packageName)) {
            std::cerr << "The dependency `" << packageName << "` could not be found in `dependencies`.\n";
            return ERR_USAGE;
        }
        std::cout << "Removing " << packageName << " from dependencies\n";
        manifest.saveFile(OKI_MANIFEST_FILE);
        config::ManifestLock manifestLock = config::ManifestLock::readOrResolve(OKI_MANIFEST_FILE, OKI_LOCK_FILE, config.getGlobalRepository());
        std::vector<std::string> directDependencies = manifest.listDeclaredPackagesNames();
        directDependencies.emplace_back(packageName);
        op::fetch(manifestLock, std::cout, {directDependencies, false});
        manifestLock.saveFile(OKI_LOCK_FILE);
        return OK;
    }

    Command RemoveAction::cmd() {
        return Command{"remove", "Remove a dependency", [](config::UserConfig &conf, ArgMatches &&args) -> std::unique_ptr<CliAction> {
                           return std::make_unique<RemoveAction>(conf, std::move(args));
                       }}
            .arg<std::string>("package", "Name of the package to remove")
            .positional("package");
    }
}
