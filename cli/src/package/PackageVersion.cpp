#include "PackageVersion.h"

namespace package {
    PackageVersion::PackageVersion(semver::Version version, std::string_view publishedDate, std::string_view downloadUrl)
        : semver::Version{version}, publishedDate{publishedDate}, downloadUrl{downloadUrl} {}

    const std::string &PackageVersion::getPublishedDate() const {
        return this->publishedDate;
    }

    const std::string &PackageVersion::getDownloadUrl() const {
        return this->downloadUrl;
    }
}
