#include "VersionLock.h"

#include <utility>

namespace package {
    VersionLock::VersionLock(const semver::Version &version, std::string_view downloadUrl, DepsType dependencies)
        : DownloadableVersion{version, downloadUrl}, dependencies{std::move(dependencies)} {}

    const VersionLock::DepsType &VersionLock::getDependencies() const {
        return dependencies;
    }
}
