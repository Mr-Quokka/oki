#include "Package.h"

namespace oki{
    Package::Package(std::string_view shortName, std::string_view longName) : shortName{shortName}, longName{longName} {}

    const std::string& Package::getShortName() const {
        return shortName;
    }

    const std::string& Package::getLongName() const {
        return longName;
    }
}