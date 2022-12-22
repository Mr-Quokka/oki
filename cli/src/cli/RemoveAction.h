#pragma once

#include "CliAction.h"

namespace cli {
    /**
     * Supprime un paquet.
     */
    class RemoveAction : public CliAction {
    private:
        std::string_view packageName;

    public:
        explicit RemoveAction(const char *packageName);
        void run(repository::Repository &repository) override;
    };
}
