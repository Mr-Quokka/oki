#pragma once

#include "CliAction.h"
#include "Command.h"

namespace cli {
    /**
     * Génère un Makefile pour compiler.
     */
    class MakefileAction : public CliAction {
    private:
        config::UserConfig &config;
        std::string fileName;

    public:
        MakefileAction(config::UserConfig &config, ArgMatches &&args);
        void run() override;
        static Command cmd();
    };
}
