#include "VersionRequirement.h"

namespace package {
    VersionRequirement::VersionRequirement(const semver::Range &range, const std::optional<std::string> &registry)
        : range{range}, registry{registry} {}

    VersionRequirement::VersionRequirement(semver::Range &&version, std::optional<std::string> &&registry)
        : range{version}, registry{std::move(registry)} {}

    const semver::Range &VersionRequirement::getRange() const {
        return range;
    }

    const std::optional<std::string> &VersionRequirement::getRegistry() const {
        return registry;
    }
}
