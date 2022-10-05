#pragma once

#include <string>

class Package {
private:
    std::string shortName;
public:
    explicit Package(std::string_view shortName);
    const std::string& getShortName() const;
};
