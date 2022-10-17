#pragma once

#include <string>

#include "Version.h"

namespace oki {
    /**
     * Un modèle de paquet pour échanger avec l'API.
     */
    class Package {
    private:
        std::string shortName;
        std::string longName;
        std::vector<Version> versions;

    public:
        Package(std::string_view shortName, std::string_view longName, std::vector<Version> versions = {});
        const std::string &getShortName() const;
        const std::string &getLongName() const;
        const std::vector<Version> &getVersions() const;
    };
}
