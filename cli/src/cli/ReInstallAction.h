#pragma once

#include "CliAction.h"

namespace oki {
    /**
     * Réinstalle tous les paquets present dans le manifeste.
     */
    class ReInstallAction : public CliAction {
    public:
        void run(Repository &repository) override;
    };
}
