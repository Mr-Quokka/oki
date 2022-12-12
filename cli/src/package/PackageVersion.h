#pragma once

#include <string>
#include <vector>

#include "DownloadableVersion.h"
#include "Summary.h"

namespace package {
    /**
     * Un modèle de version d'un paquet pour communiquer avec l'API.
     */
    class PackageVersion : public DownloadableVersion {
    private:
        std::string publishedDate;
        Summaries dependencies;

    public:
        PackageVersion(const semver::Version &version, std::string_view publishedDate, std::string_view downloadUrl, Summaries dependencies = {});
        const std::string &getPublishedDate() const;
        const package::Summaries &getDependencies() const;
        void addDependency(const std::string &name, const semver::Range &range);
    };
}
