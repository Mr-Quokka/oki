#pragma once

#include "CliAction.h"

namespace oki {
    /**
     * Liste tous les paquets disponibles.
     */
    class ListAction : public CliAction {
    public:
        ListAction();
        void run(Repository &repository) override;
    };
}