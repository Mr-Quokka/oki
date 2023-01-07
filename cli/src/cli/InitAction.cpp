#include "InitAction.h"
#include "../io/oki.h"
#include "../op/init.h"
#include <iostream>

namespace fs = std::filesystem;

namespace cli {
    InitAction::InitAction(make::ProjectKind kind)
        : projectName{fs::current_path().filename()}, projectKind{kind} {}

    InitAction::InitAction(const char *projectName, make::ProjectKind kind)
        : projectName{projectName}, projectKind{kind} {}

    InitAction::InitAction([[maybe_unused]] config::UserConfig &config, ArgMatches &&args)
        : projectName{args.get<std::string>("name").value_or(fs::current_path().filename())},
          projectKind{make::projectKindFromStr(args.require<std::string>("kind"))},
          lib{args.contains("lib")} {}

    void InitAction::run() {
        if (fs::exists(OKI_MANIFEST_FILE)) {
            std::cerr << "`oki init` cannot be run on already existing packages\n";
            exit(1);
        }
        op::init(op::InitOptions{projectName, projectKind, lib}, fs::current_path());
    }

    Command InitAction::cmd() {
        return Command{"init", "Create a new package in an existing directory",
                       [](config::UserConfig &conf, ArgMatches &&args) -> std::unique_ptr<CliAction> {
                           return std::make_unique<InitAction>(conf, std::move(args));
                       }}
            .arg<std::string>("name", "Package name to use")
            .require<std::string>("kind", "Package kind to use")
            .flag("lib", "Create a library package")
            .positional("kind");
    }
}
