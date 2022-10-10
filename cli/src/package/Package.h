#pragma once

#include <string>

class Package {
private:
    std::string shortName;
    std::string longName;
public:
    Package(std::string_view shortName, std::string_view longName);
    const std::string& getShortName() const;
    const std::string& getLongName() const;
};
