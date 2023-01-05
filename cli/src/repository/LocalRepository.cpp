#include "LocalRepository.h"
#include "../io/HttpRequest.h"
#include "../io/oki.h"

#include <algorithm>
#include <iostream>

namespace fs = std::filesystem;

namespace repository {
    LocalRepository::LocalRepository(fs::path root) : root{std::move(root)} {}

    void LocalRepository::createIfNotExists() {
        fs::create_directories(root);
    }

    std::vector<package::Package> LocalRepository::listPackages() {
        std::vector<package::Package> packages;
        for (const auto &file : fs::directory_iterator(root)) {
            packages.emplace_back(file.path().filename().string(), "");
        }
        return packages;
    }

    package::Package LocalRepository::getPackageInfo(std::string_view packageName) {

        std::vector<package::PackageVersion> versions;
        for (const auto &file : fs::directory_iterator(root / packageName)) {
            config::Manifest manifest = config::Manifest::fromFile(file.path() / OKI_MANIFEST_FILE);
            std::string fileName{packageName};
            fileName += "_";
            fileName += manifest.getPackageVersion();
            fileName += ".zip";
            versions.emplace_back(semver::Version::parse(manifest.getPackageVersion()), "",
                                  "file://" + (file.path() / fileName).string());
        }

        std::sort(versions.begin(), versions.end(), std::greater());
        return package::Package{packageName, "description", versions};
    }

    void LocalRepository::publish(config::Manifest &manifest, const std::filesystem::path &source) {
        fs::path projectDirectory = root / manifest.getProjectName() / manifest.getPackageVersion();
        fs::create_directories(projectDirectory);
        std::string projectName{manifest.getProjectName()};
        projectName += "_";
        projectName += manifest.getPackageVersion();
        projectName += ".zip";
        if (fs::exists(projectDirectory / projectName)) {
            throw io::APIException("This file already exists");
        }
        fs::copy(source, projectDirectory / projectName);
        manifest.saveFile(projectDirectory / OKI_MANIFEST_FILE);
    }
}
