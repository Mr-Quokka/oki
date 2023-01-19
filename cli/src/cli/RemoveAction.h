#pragma once

#include "CliAction.h"
#include "Command.h"

namespace cli {
    /**
     * Supprime un paquet.
     */
    class RemoveAction : public CliAction {
    private:
        config::UserConfig &config;
        std::string packageName;

    public:
        RemoveAction(config::UserConfig &config, ArgMatches &&args);
        int run() override;
        static Command cmd();
    };
}
