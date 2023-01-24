#pragma once

#include "CompilatorStrategy.h"

namespace make {
    class CppCompilatorStrategy : public CompilatorStrategy {
    public:
        std::ostream &writeMakefileStart(std::ostream &out) override;
        std::ostream &writeMakefileEnd(std::ostream &out) override;
        std::ostream &writeSampleMain(std::ostream &out) override;
        std::string_view getMainFileName() override;
    };
}
