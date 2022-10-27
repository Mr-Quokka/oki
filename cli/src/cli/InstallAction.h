#pragma once

#include "CliAction.h"

namespace oki {
    /**
     * Demande l'installation d'un paquet nommé.
     */
    class InstallAction : public CliAction {
    private:
        std::string_view packageName;

    public:
        void run(Repository &repository) override;
        explicit InstallAction(const char *packageName);
    };
}