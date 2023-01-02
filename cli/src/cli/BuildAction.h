#pragma once

#include "CliAction.h"

namespace cli {
    /**
     * Compile le projet de l'utilisateur.
     */
    class BuildAction : public CliAction {
    private:
        void bootstrap(repository::Repository &repository);

    public:
        void run(repository::Repository &repository) override;
    };
}
