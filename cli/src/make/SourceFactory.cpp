#include "SourceFactory.h"
#include "../make/CCompilatorStrategy.h"
#include "../make/CppCompilatorStrategy.h"

namespace make {
    std::unique_ptr<make::CompilatorStrategy> SourceFactory::fabrique(ProjectKind kind) {
        switch (kind) {
        case make::ProjectKind::C:
            return std::make_unique<make::CCompilatorStrategy>();
            break;
        case make::ProjectKind::Cpp:
            return std::make_unique<make::CppCompilatorStrategy>();
            break;
        default:
            return nullptr;
        }
    }
}