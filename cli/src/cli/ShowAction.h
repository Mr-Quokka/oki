#pragma once

#include "CliAction.h"
#include "Command.h"

namespace cli {
    /**
     * Affiche les informations d'un paquet d'après son nom.
     */
    class ShowAction : public CliAction {
    private:
        std::string packageName;
        repository::Repository &repository;

    public:
        ShowAction(config::UserConfig &config, ArgMatches &&args);
        void run() override;
        static Command cmd();
    };
}