#include "FetchAction.h"
#include "../config/ManifestLock.h"
#include "../io/oki.h"
#include "../op/fetch.h"

#include <iostream>

namespace cli {
    FetchAction::FetchAction(config::UserConfig &config)
        : config{config} {}

    int FetchAction::run() {
        config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
        config::ManifestLock manifestLock = config::ManifestLock::readOrResolve(OKI_MANIFEST_FILE, OKI_LOCK_FILE, config.getGlobalRepository());
        return op::fetch(manifestLock, std::cout, {manifest.listDeclaredPackagesNames(), true});
    }

    Command FetchAction::cmd() {
        return Command{"fetch", "Fetch dependencies",
                       [](config::UserConfig &conf, ArgMatches &&) -> std::unique_ptr<CliAction> {
                           return std::make_unique<FetchAction>(conf);
                       }}
            .arg<std::string>("registry", "Name of the registry to use");
    }
}
