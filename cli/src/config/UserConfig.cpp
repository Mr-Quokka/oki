#include "UserConfig.h"
#include "config.h"
#include "repository/LocalRepository.h"
#include "repository/RemoteRepository.h"
#include <toml.hpp>

namespace fs = std::filesystem;

namespace config {
    UserConfig::UserConfig(repository::GlobalRepository &&sources)
        : sources{std::move(sources)} {}

    repository::Repository &UserConfig::getRepository(std::string_view name) {
        return sources.getRepository(name);
    }

    const repository::Repository &UserConfig::getRepository(std::string_view name) const {
        return sources.getRepository(name);
    }

    repository::GlobalRepository &UserConfig::getGlobalRepository() {
        return sources;
    }

    UserConfig UserConfig::fromFile(const std::filesystem::path &path) {
        repository::GlobalRepository sources;
        if (fs::exists(path)) {
            toml::table table = toml::parse_file(path.string());
            const auto *read = table["sources"].as_table();
            if (read != nullptr) {
                for (const auto &[name, content] : *read) {
                    const auto *contentString = content.as_string();
                    if (contentString == nullptr) {
                        continue;
                    }
                    const std::string &value = contentString->get();
                    sources.addSource(name, readRepositoryPath(value));
                }
            }
        }
        sources.addDefaultSources();
        return UserConfig{std::move(sources)};
    }
}
