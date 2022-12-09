#include "PackageVersion.h"

#include <utility>

namespace package {
    PackageVersion::PackageVersion(semver::Version version, std::string_view publishedDate, std::string_view downloadUrl, Summaries dependencies)
        : semver::Version{version}, publishedDate{publishedDate}, downloadUrl{downloadUrl}, dependencies{std::move(dependencies)} {}

    const std::string &PackageVersion::getPublishedDate() const {
        return this->publishedDate;
    }

    const std::string &PackageVersion::getDownloadUrl() const {
        return this->downloadUrl;
    }

    const package::Summaries &PackageVersion::getDependencies() const {
        return this->dependencies;
    }

    void PackageVersion::addDependency(const std::string &name, const semver::Range &range) {
        this->dependencies.insert({name, range});
    }

}
