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
        package::Package getPackageInfo(std::string_view packageName) override;
        void publish(config::Manifest &manifest, const std::filesystem::path &source) override;
    };
}
