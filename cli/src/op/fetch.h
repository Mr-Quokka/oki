#pragma once

#include "../config/ManifestLock.h"
#include "../package/DownloadableVersion.h"
#include <filesystem>
#include <ostream>
#include <unordered_map>
#include <vector>

namespace op {
    struct LogOptions {
        const std::vector<std::string> &logWhenSeen;
        bool logWhenUpToDate;
    };

    int fetch(
        const config::ManifestLock &manifestLock,
        std::ostream &out,
        const LogOptions &options,
        const std::filesystem::path &workingDirectory);

    int fetch(const config::ManifestLock &manifestLock, std::ostream &out, const LogOptions &options);
}
