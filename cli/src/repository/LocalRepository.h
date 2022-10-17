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
        Package showPackage(std::string_view packageName) override;
        void download(const Version &packageVersion, const std::filesystem::path &destination) override;
    };
}
