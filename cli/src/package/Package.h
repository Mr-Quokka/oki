#pragma once

#include <string>

#include "PackageVersion.h"

namespace package {
    /**
     * Un modèle de paquet pour échanger avec l'API.
     */
    class Package {
    private:
        std::string name;
        std::string description;
        std::vector<PackageVersion> versions;

    public:
        Package(std::string_view name, std::string_view description, std::vector<PackageVersion> versions = {});
        const std::string &getName() const;
        const std::string &getDescription() const;
        const std::vector<PackageVersion> &getVersions() const;
    };
}
