#include "TreeAction.h"
#include "../config/ManifestLock.h"
#include "../io/oki.h"
#include "../tree/TreeRenderer.h"
#include "ExitStatuses.h"
#include <iostream>

namespace cli {
    TreeAction::TreeAction(config::UserConfig &config)
        : config{config} {}

    int TreeAction::run() {
        config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
        config::ManifestLock manifestLock = config::ManifestLock::readOrResolve(OKI_MANIFEST_FILE, OKI_LOCK_FILE, config.getGlobalRepository());

        std::vector<std::string> directDependencies = manifest.listDeclaredPackagesNames();
        tree::TreeRenderer renderer{manifestLock, directDependencies};
        std::cout << renderer;
        return OK;
    }

    Command TreeAction::cmd() {
        return Command{"tree", "Display a tree visualization of the dependency graph",
                       [](config::UserConfig &conf, ArgMatches &&) -> std::unique_ptr<CliAction> {
                           return std::make_unique<TreeAction>(conf);
                       }};
    }
}
