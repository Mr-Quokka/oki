#pragma once

#include <string>
#include <vector>

#include "../package/Summary.h"
#include "../semver/Version.h"

namespace package {
    /**
     * Un modèle de version d'un paquet pour communiquer avec l'API.
     */
    class PackageVersion : public semver::Version {
    private:
        std::string publishedDate;
        std::string downloadUrl;
        Summaries dependencies;

    public:
        PackageVersion(semver::Version version, std::string_view publishedDate, std::string_view downloadUrl, Summaries dependencies = {});
        const std::string &getPublishedDate() const;
        const std::string &getDownloadUrl() const;
        const package::Summaries &getDependencies() const;
        void addDependency(std::string name, semver::Range range);
    };
}
