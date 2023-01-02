#pragma once

#include "../make/ProjectKind.h"
#include <filesystem>
#include <ostream>

namespace op {
    struct InitOptions {
        std::string projectName;
        make::ProjectKind kind;
        bool lib;
    };

    int init(
        const InitOptions &options,
        const std::filesystem::path &workingDirectory);
}
