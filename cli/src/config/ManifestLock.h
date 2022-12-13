#pragma once

#include "../package/VersionLock.h"
#include "../repository/Repository.h"
#include "../semver/Version.h"
#include "../solver/Resolver.h"
#include <filesystem>
#include <string>
#include <unordered_map>

namespace config {
    class ManifestLock {
    private:
        using Packages = std::unordered_map<std::string, package::VersionLock>;

        Packages locks;

    public:
        explicit ManifestLock(Packages resolved);
        explicit ManifestLock(const solver::Resolved &resolved);

        const Packages &getLocks() const;

        static ManifestLock fromFile(const std::filesystem::path &fileName);

        static ManifestLock readOrResolve(const std::filesystem::path &manifestFileName, const std::filesystem::path &lockFileName, repository::Repository &repository);

        void saveFile(const std::filesystem::path &fileName) const;
    };

    std::ostream &operator<<(std::ostream &os, const ManifestLock &lock);
}
