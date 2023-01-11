#pragma once

#include "../config/UserConfig.h"
#include "CliAction.h"
#include "Command.h"

namespace cli {
    class PackageAction : public CliAction {
    private:
        config::UserConfig &config;
        bool list;
        bool noVerify;

    public:
        PackageAction(config::UserConfig &config, ArgMatches &&args);

        int run() override;

        static Command cmd();
    };
}
