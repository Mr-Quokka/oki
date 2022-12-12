#include "PackageVersion.h"

#include <utility>

namespace package {
    PackageVersion::PackageVersion(const semver::Version &version, std::string_view publishedDate, std::string_view downloadUrl, Summaries dependencies)
        : DownloadableVersion{version, downloadUrl}, publishedDate{publishedDate}, dependencies{std::move(dependencies)} {}

    const std::string &PackageVersion::getPublishedDate() const {
        return this->publishedDate;
    }

    const package::Summaries &PackageVersion::getDependencies() const {
        return this->dependencies;
    }

    void PackageVersion::addDependency(const std::string &name, const semver::Range &range) {
        this->dependencies.insert({name, range});
    }

}
