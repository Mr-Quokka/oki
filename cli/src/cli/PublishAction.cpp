#include "PublishAction.h"

#include "../io/TmpFile.h"
#include "../io/oki.h"
#include "../op/package.h"

namespace fs = std::filesystem;

namespace cli {
    PublishAction::PublishAction(config::UserConfig &config, ArgMatches &&args)
        : registry{config.getRepository(args.get<std::string>("registry").value_or("default"))} {}

    void PublishAction::run() {
        io::TmpFile tmp;
        op::package(tmp.path(), fs::current_path());
        config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
        registry.publish(manifest, tmp.path());
    }

    Command PublishAction::cmd() {
        return Command{"publish", "Upload a package to a repository", [](config::UserConfig &conf, ArgMatches &&args) -> std::unique_ptr<CliAction> {
                           return std::make_unique<PublishAction>(conf, std::move(args));
                       }}
            .arg<std::string>("registry", "Name of the registry to publish to");
    }
}
