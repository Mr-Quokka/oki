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

    bool acceptColor() {
        return isatty(STDOUT_FILENO);
    }
}
