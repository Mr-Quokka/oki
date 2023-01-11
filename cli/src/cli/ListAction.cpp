#include "ListAction.h"

#include <iostream>

namespace cli {
    ListAction::ListAction(config::UserConfig &config, ArgMatches &&args)
        : repository{args.getRegistry(config)} {}

    int ListAction::run() {
        for (const package::Package &package : repository.listPackages()) {
            std::cout << package.getName() << ": " << package.getDescription() << "\n";
        }
    }

    Command ListAction::cmd() {
        return Command{"list", "List available packages", [](config::UserConfig &conf, ArgMatches &&args) -> std::unique_ptr<CliAction> {
                           return std::make_unique<ListAction>(conf, std::move(args));
                       }}
            .arg<std::string>("registry", "Name of the registry to use");
    }
}
