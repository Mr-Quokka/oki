#pragma once

#include "CliAction.h"

namespace cli {
    /**
     * Génère un Makefile pour compiler.
     */
    class MakefileAction : public CliAction {
    private:
        std::string_view fileName;

    public:
        explicit MakefileAction(const char *fileName = "Makefile");
        void run(repository::Repository &repository) override;
    };
}