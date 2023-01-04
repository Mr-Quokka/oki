#pragma once

#include <ostream>

namespace make {
    class CompilatorStrategy {
    public:
        virtual std::ostream &writeStart(std::ostream &out) = 0;
        virtual std::ostream &writeEnd(std::ostream &out) = 0;
        virtual ~CompilatorStrategy() = default;
    };
}
