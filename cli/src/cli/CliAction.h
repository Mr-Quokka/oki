#pragma once

#include "../repository/Repository.h"

namespace cli {
    class CliAction {
    public:
        virtual void run(repository::Repository &repository) = 0;
        virtual ~CliAction() = default;
    };
}