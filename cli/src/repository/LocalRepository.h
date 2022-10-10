#pragma once

#include "Repository.h"

namespace oki{
    class LocalRepository : public Repository {
    private:
        std::filesystem::path root;
    public:
        explicit LocalRepository(std::filesystem::path root);
        void createIfNotExists();
        std::vector<Package> listPackages() override;
        std::optional<Package> showPackage(std::string_view packageName) override;
        void download(std::string_view packageName, const std::filesystem::path& destination) override;
    };
}
