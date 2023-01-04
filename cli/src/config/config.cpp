#include "config.h"

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
        if(okiRepository == nullptr) {
            return "https://oki-pkg.dev";
        }
        return okiRepository;
    }

    bool acceptColor() {
        return isatty(STDOUT_FILENO);
    }
}
