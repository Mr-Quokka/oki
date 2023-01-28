#pragma once

#include "../config/AuthProvider.h"
#include "Repository.h"

namespace repository {
    class RemoteRepository : public Repository {
    private:
        std::string apiUrl;
        config::AuthProvider authProvider;

    public:
        explicit RemoteRepository(std::string_view apiUrl);
        std::vector<package::Package> listPackages() override;
        package::Package getPackageInfo(std::string_view packageName) override;
        bool publish(config::Manifest &manifest, const std::filesystem::path &source) override;
    };
}
