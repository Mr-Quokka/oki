#include "config.h"
#include "../repository/LocalRepository.h"
#include "../repository/RemoteRepository.h"

#include <unistd.h>

namespace fs = std::filesystem;

namespace config {
    fs::path getDefaultLocalRepository() {
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        const char *xdgData = std::getenv("XDG_DATA_HOME");
        if (xdgData == nullptr) {
            return std::getenv("HOME") / fs::path{".local/share/oki"};
        }
        return xdgData / fs::path{"oki"};
    }

    std::string_view getDefaultRemoteRepository() {
        const char *okiRepository = std::getenv("OKI_REPOSITORY");
        if (okiRepository == nullptr) {
            return "https://oki-pkg.dev";
        }
        return okiRepository;
    }

    fs::path getConfigPath() {
        const char *xdgData = std::getenv("XDG_CONFIG_HOME");
        if (xdgData == nullptr) {
            return std::getenv("HOME") / fs::path{".config/oki.toml"};
        }
        return xdgData / fs::path{"oki.toml"};
    }

    std::unique_ptr<repository::Repository> readRepositoryPath(std::string_view source) {
        if (source.starts_with("local:")) {
            return std::make_unique<repository::LocalRepository>(repository::LocalRepository{source.substr(6)});
        }
        return std::make_unique<repository::RemoteRepository>(repository::RemoteRepository{source});
    }

    bool acceptColor() {
        return isatty(STDOUT_FILENO);
    }
}
