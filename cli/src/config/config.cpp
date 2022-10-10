#include "config.h"

#include <cstdlib>
#include <unistd.h>

namespace fs = std::filesystem;

namespace oki{
    fs::path getDefaultLocalRepository() {
        // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
        const char *xdgData = std::getenv("XDG_DATA_HOME");
        if (xdgData == nullptr) {
            return std::getenv("HOME") / fs::path{".local/share/oki"};
        }
        return xdgData / fs::path{"oki"};
    }

    bool acceptColor() {
        return isatty(fileno(stdout));
    }
}
