#include "Package.h"

#include <utility>

namespace package {
    Package::Package(std::string_view name, std::string_view description, std::vector<PackageVersion> versions)
        : name{name}, description{description}, versions{std::move(versions)} {}

    const std::string &Package::getName() const {
        return name;
    }

    const std::string &Package::getDescription() const {
        return description;
    }

    const std::vector<PackageVersion> &Package::getVersions() const {
        return this->versions;
    }
}
