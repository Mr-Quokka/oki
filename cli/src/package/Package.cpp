#include "Package.h"

#include <utility>

namespace oki {
    Package::Package(std::string_view shortName, std::string_view description, std::vector<Version> versions)
        : shortName{shortName}, description{description}, versions{std::move(versions)} {}

    const std::string &Package::getShortName() const {
        return shortName;
    }

    const std::string &Package::getDescription() const {
        return description;
    }

    const std::vector<Version> &Package::getVersions() const {
        return this->versions;
    }
}
