#include "ProjectKind.h"

namespace make {
    std::ostream &operator<<(std::ostream &out, ProjectKind kind) {
        switch (kind) {
        case ProjectKind::C:
            out << "c";
            break;
        case ProjectKind::Cpp:
            out << "cpp";
            break;
        default:
            out << "unknown";
            break;
        }
        return out;
    }

    std::optional<ProjectKind> tryProjectKindFromStr(std::string_view str) {
        if (str == "c") {
            return ProjectKind::C;
        } else if (str == "cpp") {
            return ProjectKind::Cpp;
        }
        return {};
    }

    ProjectKind projectKindFromStr(std::string_view str) {
        std::optional<make::ProjectKind> kindOpt = make::tryProjectKindFromStr(str);
        if (kindOpt.has_value()) {
            return *kindOpt;
        }
        throw std::runtime_error{"Supported project kind values are `c` or `cpp` but `" + std::string{str} + "` is unknown"};
    }
}