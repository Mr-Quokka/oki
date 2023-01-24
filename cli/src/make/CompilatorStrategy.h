#pragma once

#include <ostream>

namespace make {
    class CompilatorStrategy {
    public:
        virtual std::ostream &writeMakefileStart(std::ostream &out) = 0;
        virtual std::ostream &writeMakefileEnd(std::ostream &out) = 0;
        virtual std::ostream &writeSampleMain(std::ostream &out) = 0;
        virtual std::string_view getMainFileName() = 0;
        virtual ~CompilatorStrategy() = default;
    };
}
