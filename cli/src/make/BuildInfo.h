#pragma once

#include <optional>
#include <string>

namespace make {
    struct BuildInfo {
        std::optional<std::string> buildStatic;
        std::vector<std::string> exports;
        std::optional<std::string> staticLink;
    };
}
