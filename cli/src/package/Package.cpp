#include "Package.h"

Package::Package(std::string_view shortName) : shortName{shortName} {}

const std::string& Package::getShortName() const {
    return shortName;
}
