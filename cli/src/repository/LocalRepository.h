#pragma once

#include "Repository.h"

namespace oki {
    class LocalRepository : public Repository {
    private:
        std::filesystem::path root;

    public:
        explicit LocalRepository(std::filesystem::path root);
        void createIfNotExists();
        std::vector<Package> listPackages() override;
        Package getPackageInfo(std::string_view packageName) override;
        std::string getPackageURL(std::string_view packageName, std::string packageVersion) override;
        void download(const Version &packageVersion, const std::filesystem::path &destination) override;
    };
}
