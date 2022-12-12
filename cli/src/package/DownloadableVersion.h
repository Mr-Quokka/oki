#pragma once

#include "../io/Downloadable.h"
#include "../semver/Version.h"

namespace package {
    /**
     * Une version qu'il est possible de télécharger.
     */
    class DownloadableVersion : public semver::Version, public io::Downloadable {
    public:
        DownloadableVersion(const semver::Version &version, const io::Downloadable &downloadable);
        DownloadableVersion(const semver::Version &version, std::string_view downloadUrl);
    };
}
