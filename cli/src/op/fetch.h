#pragma once

#include "package/DownloadableVersion.h"
#include <filesystem>
#include <ostream>
#include <unordered_map>
#include <vector>

namespace op {
    template <typename T>
    concept DownloadableVersionConcept = std::is_base_of_v<package::DownloadableVersion, T>;

    struct LogOptions {
        const std::vector<std::string> &logWhenSeen;
        bool logWhenUpToDate;
    };

    template <DownloadableVersionConcept T>
    int fetch(
        const std::unordered_map<std::string, T> &resolved,
        std::ostream &out,
        const LogOptions &options,
        const std::filesystem::path &workingDirectory);

    template <DownloadableVersionConcept T>
    int fetch(const std::unordered_map<std::string, T> &resolved, std::ostream &out, const LogOptions &options) {
        return fetch(resolved, out, options, std::filesystem::current_path());
    }
}
