#pragma once

#include "CliAction.h"

namespace cli {
    class TreeAction : public CliAction {
    public:
        void run(repository::Repository &repository) override;
    };
}
