#include "InstallAction.h"
#include "../config/ManifestLock.h"
#include "../io/HttpRequest.h"
#include "../io/oki.h"
#include "../op/fetch.h"
#include "../repository/RepositoryException.h"
#include "ExitStatuses.h"
#include <iostream>

namespace cli {
    InstallAction::InstallAction(config::UserConfig &config, ArgMatches &&args)
        : packageName{args.require<std::string>("package")}, registry{args.get<std::string>("registry")}, repository{config.getGlobalRepository()} {}

    int InstallAction::run() {
        package::Package p = repository.getPackageInfo(packageName);
        if (p.getVersions().empty()) {
            throw repository::RepositoryException{"The package doesn't have any version"};
        } else {
            config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
            package::PackageVersion latest = p.getVersions().front();
            bool success = manifest.addDeclaredPackage(packageName, package::VersionRequirement{semver::Range::featureSatisfying(latest), registry});

            solver::Resolved resolved = solver::resolve(manifest.listDeclaredPackages(), repository);
            if (success) {
                std::cout << "Adding " << packageName << " v" << latest << " to dependencies.\n";
            }
            manifest.saveFile(OKI_MANIFEST_FILE);
            config::ManifestLock lock{resolved};
            op::fetch(lock, std::cout, {{std::string{packageName}}, true});
            lock.saveFile(OKI_LOCK_FILE);
        }
        return OK;
    }

    Command InstallAction::cmd() {
        return Command{"install", "Install a dependency",
                       [](config::UserConfig &config, ArgMatches &&args) -> std::unique_ptr<CliAction> {
                           return std::make_unique<InstallAction>(config, std::move(args));
                       }}
            .arg<std::string>("registry", "Name of the registry to use")
            .arg<std::string>("package", "Name of the package to install")
            .positional("package");
    }
}
