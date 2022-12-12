#include "DownloadableVersion.h"

namespace package {
    DownloadableVersion::DownloadableVersion(const semver::Version &version, const io::Downloadable &downloadable)
        : semver::Version{version}, io::Downloadable{downloadable} {}

    DownloadableVersion::DownloadableVersion(const semver::Version &version, std::string_view downloadUrl)
        : semver::Version{version}, io::Downloadable{downloadUrl} {}
}
