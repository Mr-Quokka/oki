#pragma once

#include "CompilatorStrategy.h"

namespace make {
    class CCompilatorStrategy : public CompilatorStrategy {
    public:
        std::ostream &writeStart(std::ostream &out) override;
        std::ostream &writeEnd(std::ostream &out) override;
        std::ostream &writeMain(std::ostream &out) override;
    };
}
