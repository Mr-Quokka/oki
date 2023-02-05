#include "ListAction.h"

#include "ExitStatuses.h"
#include <iostream>

namespace cli {
    ListAction::ListAction(config::UserConfig &config, ArgMatches &&args)
        : repository{args.getRegistry(config)},
          params{.page = args.get<unsigned int>("page").value_or(1),
                 .limit = args.get<unsigned int>("limit").value_or(10)} {}

    int ListAction::run() {
        for (const package::Package &package : repository.listPackages(params)) {
            std::cout << package.getName() << ": " << package.getDescription() << "\n";
        }
        return OK;
    }

    Command ListAction::cmd() {
        return Command{"list", "List available packages", [](config::UserConfig &conf, ArgMatches &&args) -> std::unique_ptr<CliAction> {
                           return std::make_unique<ListAction>(conf, std::move(args));
                       }}
            .arg<unsigned int>("page", "The page to display")
            .arg<unsigned int>("limit", "Limit the number of results")
            .arg<std::string>("registry", "Name of the registry to use");
    }
}
