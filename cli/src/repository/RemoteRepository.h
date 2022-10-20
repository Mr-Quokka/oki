#pragma once

#include "Repository.h"

namespace oki {
    class RemoteRepository : public Repository {
    private:
        std::string apiUrl;

    public:
        explicit RemoteRepository(std::string_view apiUrl);
        std::vector<Package> listPackages() override;
        std::optional<Package> showPackage(std::string_view packageName) override;
        void download(const Version &packageVersion, const std::filesystem::path &destination) override;
    };
}