#pragma once

#include "DownloadableVersion.h"
#include <vector>

namespace package {
    class VersionLock : public DownloadableVersion {
    public:
        using DepsType = std::vector<std::string>;

    private:
        DepsType dependencies;

    public:
        VersionLock(const semver::Version &version, std::string_view downloadUrl, DepsType dependencies = {});

        const DepsType &getDependencies() const;
    };
}
