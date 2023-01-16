#include "options.h"

#include <iostream>

#include "BuildAction.h"
#include "FetchAction.h"
#include "InitAction.h"
#include "InstallAction.h"
#include "ListAction.h"
#include "MakefileAction.h"
#include "PackageAction.h"
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
            {"fetch", FetchAction::cmd()},
            {"install", InstallAction::cmd()},
            {"remove", RemoveAction::cmd()},
            {"tree", TreeAction::cmd()},
            {"build", BuildAction::cmd()},
            {"makefile", MakefileAction::cmd()},
            {"package", PackageAction::cmd()},
            {"publish", PublishAction::cmd()},
        };
        if (strcmp("help", args[1]) == 0) {
            help(std::cout, commands);
            return nullptr;
        }
        auto it = commands.find(args[1]);
        if (it == commands.end()) {
            std::cerr << "Unknown action.\n";
            return nullptr;
        }
        return it->second.create(config, args.subspan(1));
    }
}
