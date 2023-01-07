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
        std::string_view packageName;

    public:
        RemoveAction(config::UserConfig &config, ArgMatches &&args);
        void run() override;
        static Command cmd();
    };
}
