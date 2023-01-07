#pragma once

#include "../repository/Repository.h"
#include "CliAction.h"
#include "Command.h"

namespace cli {
    /**
     * Réinstalle tous les paquets présents dans le manifeste qui ne sont pas à jour.
     */
    class FetchAction : public CliAction {
    private:
        config::UserConfig &config;

    public:
        explicit FetchAction(config::UserConfig &config);

        void run() override;

        static Command cmd();
    };
}
