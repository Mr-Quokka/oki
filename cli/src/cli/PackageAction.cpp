#include "PackageAction.h"
#include "../io/oki.h"
#include "../op/package.h"
#include "../op/verify.h"
#include "ExitStatuses.h"
#include <iostream>

namespace fs = std::filesystem;

namespace cli {
    PackageAction::PackageAction(config::UserConfig &config, ArgMatches &&args)
        : config{config}, list{args.contains("list")}, noVerify{args.contains("no-verify")} {}

    int PackageAction::run() {
        config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
        fs::path relativeTo = fs::current_path();
        std::vector<fs::path> include = op::listPackagedFiles(manifest.getInclude(), std::cerr, relativeTo);
        if (!noVerify && !op::verify(manifest, include)) {
            return ERR_CONFIG;
        }
        if (list) {
            for (const auto &path : include) {
                std::cout << fs::relative(path, relativeTo).string() << "\n";
            }
            return OK;
        }
        fs::path archivePath = manifest.getPackageArchive();
        op::package(archivePath, relativeTo, include);
        std::cout << "Created " << archivePath.string() << "\n";
        return OK;
    }

    Command PackageAction::cmd() {
        return Command{"package", "Assemble the current package into a distributable tarball",
                       [](config::UserConfig &conf, ArgMatches &&args) -> std::unique_ptr<CliAction> {
                           return std::make_unique<PackageAction>(conf, std::move(args));
                       }}
            .flag("list", "Print files included in a package without making one")
            .flag("no-verify", "Don't verify the package when building it");
    }
}
