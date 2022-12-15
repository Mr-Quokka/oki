#pragma once

#include "CliAction.h"

namespace cli {
    /**
     * Affiche les informations d'un paquet d'après son nom.
     */
    class MakefileAction : public CliAction {
    private:
        std::string_view fileName;

    public:
        explicit MakefileAction(const char *fileName = "Makefile");
        void run(repository::Repository &repository) override;
    };
}