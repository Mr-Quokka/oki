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
    public:
        using Packages = std::unordered_map<std::string, package::VersionLock>;

    private:
        Packages locks;

    public:
        ManifestLock() = default;
        explicit ManifestLock(Packages resolved);
        explicit ManifestLock(const solver::Resolved &resolved);

        const Packages &getLocks() const;

        bool contains(const std::string &packageName) const;
        bool containsExact(const std::string &packageName, const package::DownloadableVersion &version) const;

        Packages::const_iterator find(const std::string &packageName) const;
        const package::VersionLock &at(const std::string &packageName) const;

        Packages::const_iterator cbegin() const;

        Packages::const_iterator cend() const;

        static ManifestLock fromFile(const std::filesystem::path &fileName);
        static ManifestLock fromFileOrEmpty(const std::filesystem::path &fileName);

        static ManifestLock readOrResolve(const std::filesystem::path &manifestFileName, const std::filesystem::path &lockFileName, repository::GlobalRepository &repository);

        void saveFile(const std::filesystem::path &fileName) const;
    };

    std::ostream &operator<<(std::ostream &os, const ManifestLock &lock);
}
