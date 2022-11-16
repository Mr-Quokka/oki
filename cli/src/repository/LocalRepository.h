#pragma once

#include "Repository.h"

namespace repository {
    class LocalRepository : public Repository {
    private:
        std::filesystem::path root;

    public:
        explicit LocalRepository(std::filesystem::path root);
        void createIfNotExists();
        std::vector<package::Package> listPackages() override;
        std::optional<package::Package> showPackage(std::string_view packageName) override;
        std::string getPackageURL(std::string_view packageName, std::string packageVersion) override;
        void download(const package::Version &packageVersion, const std::filesystem::path &destination) override;
    };
}
