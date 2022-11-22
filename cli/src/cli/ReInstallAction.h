#pragma once

#include "CliAction.h"

namespace cli {
    /**
     * Réinstalle tous les paquets present dans le manifeste.
     */
    class ReInstallAction : public CliAction {
    public:
        void run(repository::Repository &repository) override;
    };
}
