#pragma once

#include "CliAction.h"
#include "Command.h"

namespace cli {
    /**
     * Compile le projet de l'utilisateur.
     */
    class BuildAction : public CliAction {
    private:
        config::UserConfig &config;

        void bootstrap();

    public:
        explicit BuildAction(config::UserConfig &config);

        int run() override;

        static Command cmd();
    };
}
