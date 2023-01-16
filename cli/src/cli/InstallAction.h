#pragma once

#include "ArgMatches.h"
#include "CliAction.h"
#include "Command.h"

namespace cli {
    /**
     * Demande l'installation d'un paquet nommé.
     */
    class InstallAction : public CliAction {
    private:
        std::string packageName;
        repository::Repository &repository;

    public:
        InstallAction(config::UserConfig &config, ArgMatches &&args);

        int run() override;

        static Command cmd();
    };
}