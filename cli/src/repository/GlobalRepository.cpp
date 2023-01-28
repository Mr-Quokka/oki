#include "GlobalRepository.h"
#include "../config/config.h"
#include "../io/HttpRequest.h"
#include "LocalRepository.h"
#include "RemoteRepository.h"
#include "RepositoryException.h"

namespace repository {
    repository::Repository &GlobalRepository::getRepository(std::string_view name) {
        auto it = std::find_if(sources.begin(), sources.end(), [&name](const RepositoryId &id) {
            return id.name == name;
        });
        if (it != sources.end()) {
            return *it->repository;
        }
        throw std::invalid_argument("Repository not found");
    }

    const repository::Repository &GlobalRepository::getRepository(std::string_view name) const {
        auto it = std::find_if(sources.cbegin(), sources.cend(), [&name](const RepositoryId &id) {
            return id.name == name;
        });
        if (it != sources.cend()) {
            return *it->repository;
        }
        throw RepositoryException{"Repository `" + std::string{name} + "` not found"};
    }

    std::vector<package::Package> GlobalRepository::listPackages() {
        return sources.front().repository->listPackages();
    }

    package::Package GlobalRepository::getPackageInfo(std::string_view packageName) {
        for (const auto &source : sources) {
            try {
                return source.repository->getPackageInfo(packageName);
            } catch (const RepositoryException &) {
            }
        }
        throw RepositoryException{"Unable to find package on any repository"};
    }

    bool GlobalRepository::publish(config::Manifest &manifest, const std::filesystem::path &source) {
        return sources.front().repository->publish(manifest, source);
    }

    bool GlobalRepository::addSource(std::string_view name, std::unique_ptr<repository::Repository> &&repository) {
        if (std::find_if(sources.begin(), sources.end(), [&name](const RepositoryId &id) {
                return id.name == name;
            }) != sources.end()) {
            return false;
        }
        sources.emplace_back(repository::RepositoryId{
            std::string{name},
            std::move(repository),
        });
        return true;
    }

    void GlobalRepository::addDefaultSources() {
        addSource(
            "default",
            std::make_unique<RemoteRepository>(config::getDefaultRemoteRepository()));
        addSource(
            "local",
            std::make_unique<LocalRepository>(config::getDefaultLocalRepository()));
    }
}
