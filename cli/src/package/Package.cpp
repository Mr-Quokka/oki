#include "Package.h"

#include <utility>

namespace package {
    Package::Package(std::string_view shortName, std::string_view description, std::vector<PackageVersion> versions)
        : shortName{shortName}, description{description}, versions{std::move(versions)} {}

    const std::string &Package::getShortName() const {
        return shortName;
    }

    const std::string &Package::getDescription() const {
        return description;
    }

    const std::vector<PackageVersion> &Package::getVersions() const {
        return this->versions;
    }
}
