#pragma once

#include <optional>
#include <ostream>

namespace make {
    enum class ProjectKind : unsigned char {
        C,
        Cpp
    };

    std::ostream &operator<<(std::ostream &out, ProjectKind kind);

    std::optional<ProjectKind> tryProjectKindFromStr(std::string_view str);

    ProjectKind projectKindFromStr(std::string_view str);
}
