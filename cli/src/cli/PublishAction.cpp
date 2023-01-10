#include "PublishAction.h"

#include "../io/TmpFile.h"
#include "../io/oki.h"
#include "../op/package.h"
#include "../op/verify.h"
#include <iostream>

namespace fs = std::filesystem;

namespace cli {
    PublishAction::PublishAction(config::UserConfig &config, ArgMatches &&args)
        : registry{config.getRepository(args.get<std::string>("registry").value_or("default"))} {}

    void PublishAction::run() {
        io::TmpFile tmp;
        config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
        std::vector<fs::path> include = op::listPackagedFiles(manifest.getInclude(), std::cerr, fs::current_path());
        if (!op::verify(manifest, include)) {
            exit(1);
        }
        op::package(tmp.path(), fs::current_path(), include);
        registry.publish(manifest, tmp.path());
        std::cout << manifest.getProjectName() << " has been published.\n";
    }

    Command PublishAction::cmd() {
        return Command{"publish", "Upload a package to a repository",
                       [](config::UserConfig &conf, ArgMatches &&args) -> std::unique_ptr<CliAction> {
                           return std::make_unique<PublishAction>(conf, std::move(args));
                       }}
            .arg<std::string>("registry", "Name of the registry to publish to");
    }
}
