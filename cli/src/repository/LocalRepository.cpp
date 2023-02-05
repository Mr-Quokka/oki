#include "LocalRepository.h"
#include "../io/HttpRequest.h"
#include "../io/oki.h"
#include "RepositoryException.h"

#include <algorithm>

namespace fs = std::filesystem;

namespace repository {
    LocalRepository::LocalRepository(fs::path root) : root{std::move(root)} {}

    void LocalRepository::createIfNotExists() {
        fs::create_directories(root);
    }

    std::vector<package::Package> LocalRepository::listPackages(SearchParameters &params) {
        std::vector<package::Package> packages;
        for (const auto &file : fs::directory_iterator(root)) {
            packages.emplace_back(file.path().filename().string(), "");
        }
        ptrdiff_t start = static_cast<ptrdiff_t>(std::min(packages.size(), static_cast<size_t>((params.page - 1) * params.limit)));
        ptrdiff_t end = static_cast<ptrdiff_t>(std::min(packages.size(), static_cast<size_t>(params.page * params.limit)));
        return {packages.begin() + start, packages.begin() + end};
    }

    package::Package LocalRepository::getPackageInfo(std::string_view packageName) {
        fs::path packagePath{root / packageName};
        if (!fs::exists(packagePath)) {
            throw repository::RepositoryException{"This package does not exist"};
        }
        std::vector<package::PackageVersion> versions;
        for (const auto &file : fs::directory_iterator(packagePath)) {
            config::Manifest manifest = config::Manifest::fromFile(file.path() / OKI_MANIFEST_FILE);
            std::string fileName{packageName};
            fileName += "_";
            fileName += manifest.getPackageVersion();
            fileName += ".zip";
            versions.emplace_back(manifest.getPackageVersion(), "",
                                  "file://" + (file.path() / fileName).string());
        }

        std::sort(versions.begin(), versions.end(), std::greater());
        return package::Package{packageName, "description", versions};
    }

    bool LocalRepository::publish(config::Manifest &manifest, const std::filesystem::path &source) {
        fs::path projectDirectory = root / manifest.getProjectName() / manifest.getPackageVersion().str();
        fs::create_directories(projectDirectory);
        std::string projectName{manifest.getProjectName()};
        projectName += "_";
        projectName += manifest.getPackageVersion();
        projectName += ".zip";
        if (fs::exists(projectDirectory / projectName)) {
            throw repository::RepositoryException{"This file already exists"};
        }
        fs::copy(source, projectDirectory / projectName);
        manifest.saveFile(projectDirectory / OKI_MANIFEST_FILE);
        return true;
    }
}
