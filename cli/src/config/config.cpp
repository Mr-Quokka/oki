#include "config.h"

#include <cstdlib>

namespace fs = std::filesystem;

fs::path getDefaultLocalRepository() {
    // https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
	const char* xdgData = std::getenv("XDG_DATA_HOME");
    if (xdgData == nullptr) {
        return std::getenv("HOME") / fs::path{".local/share/oki"};
    }
    return xdgData / fs::path{"oki"};
}
