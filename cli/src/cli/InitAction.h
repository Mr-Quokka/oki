#pragma once

#include "../make/ProjectKind.h"
#include "ArgMatches.h"
#include "CliAction.h"
#include "Command.h"

namespace cli {
    class InitAction : public CliAction {
    private:
        std::string projectName;
        make::ProjectKind projectKind;
        bool lib;

    public:
        explicit InitAction(make::ProjectKind kind);
        InitAction(const char *projectName, make::ProjectKind kind);
        InitAction(config::UserConfig &config, ArgMatches &&args);

        int run() override;

        static Command cmd();
    };
}
