#include "options.h"

#include <iostream>

#include "BuildAction.h"
#include "FetchAction.h"
#include "InitAction.h"
#include "InstallAction.h"
#include "ListAction.h"
#include "MakefileAction.h"
#include "PublishAction.h"
#include "RemoveAction.h"
#include "ShowAction.h"
#include "TreeAction.h"

namespace cli {
    void help(std::ostream &os, const std::map<std::string_view, Command> &commands) {
        for (const auto &[name, command] : commands) {
            os << name << ": " << command.getDescription() << "\n";
        }
    }

    std::unique_ptr<CliAction> parseArguments(config::UserConfig &config, std::span<const char *> args) {
        const std::map<std::string_view, Command> commands{
            {"init", InitAction::cmd()},
            {"list", ListAction::cmd()},
            {"show", ShowAction::cmd()},
            {"fetch", PublishAction::cmd()},
            {"install", InstallAction::cmd()},
            {"remove", RemoveAction::cmd()},
            {"tree", TreeAction::cmd()},
            {"build", BuildAction::cmd()},
            {"makefile", MakefileAction::cmd()},
            {"publish", PublishAction::cmd()},
        };
        auto it = commands.find(args[1]);
        if (it == commands.end()) {
            help(std::cerr, commands);
            exit(1);
        }
        return it->second.create(config, args.subspan(1));
    }
}
