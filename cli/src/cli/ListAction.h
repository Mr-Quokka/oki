#pragma once

#include "CliAction.h"

namespace cli {
    /**
     * Liste tous les paquets disponibles.
     */
    class ListAction : public CliAction {
    public:
        void run(repository::Repository &repository) override;
    };
}
