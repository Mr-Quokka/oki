#pragma once

#include "CliAction.h"

namespace oki {
    /**
     * Liste tous les paquets disponibles.
     */
    class ListAction : public CliAction {
    public:
        void run(Repository &repository) override;
    };
}
