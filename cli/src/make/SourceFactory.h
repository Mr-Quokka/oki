#pragma once

#include "../make/CompilatorStrategy.h"
#include "ProjectKind.h"
#include <memory>

namespace make {
    class SourceFactory {
    public:
        static std::unique_ptr<make::CompilatorStrategy> fabrique(ProjectKind kind);
    };
}
