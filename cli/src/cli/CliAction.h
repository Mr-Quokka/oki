#pragma once

#include "../repository/Repository.h"

namespace oki {
    class CliAction {
    public:
        virtual void run(Repository &repository) = 0;
        virtual ~CliAction() = default;
    };
}