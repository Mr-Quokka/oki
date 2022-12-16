#pragma once

#include "DownloadableVersion.h"
#include <vector>

namespace package {
    /**
     * Les informations verouillées sur une version précise.
     */
    class VersionLock : public DownloadableVersion {
    public:
        using DepsType = std::vector<std::string>;

    private:
        DepsType dependencies;

    public:
        VersionLock(const semver::Version &version, std::string_view downloadUrl, DepsType dependencies = {});

        /**
         * Récupère les noms des paquets sur lesquels la version dépend.
         */
        const DepsType &getDependencies() const;
    };
}
