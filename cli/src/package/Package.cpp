#include "Package.h"

namespace oki {
    Package::Package(std::string_view shortName, std::string_view longName, std::vector<Version> versions)
        : shortName{shortName}, longName{longName}, versions{versions} {}

    const std::string &Package::getShortName() const {
        return shortName;
    }

    const std::string &Package::getLongName() const {
        return longName;
    }

    const std::vector<Version> &Package::getVersions() const {
        return this->versions;
    }
}