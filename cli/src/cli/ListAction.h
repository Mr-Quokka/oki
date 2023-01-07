#pragma once

#include "ArgMatches.h"
#include "CliAction.h"
#include "Command.h"

namespace cli {
    /**
     * Liste tous les paquets disponibles.
     */
    class ListAction : public CliAction {
    private:
        repository::Repository &repository;

    public:
        ListAction(config::UserConfig &config, ArgMatches &&args);

        void run() override;

        static Command cmd();
    };
}
