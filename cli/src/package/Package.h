#pragma once

#include <string>

#include "Version.h"

namespace package {
    /**
     * Un modèle de paquet pour échanger avec l'API.
     */
    class Package {
    private:
        std::string shortName;
        std::string description;
        std::vector<Version> versions;

    public:
        Package(std::string_view shortName, std::string_view description, std::vector<Version> versions = {});
        const std::string &getShortName() const;
        const std::string &getDescription() const;
        const std::vector<Version> &getVersions() const;
    };
}
