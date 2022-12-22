#pragma once

#include "package/DownloadableVersion.h"
#include <filesystem>
#include <ostream>
#include <unordered_map>
#include <vector>

namespace op {
    template <typename T>
    concept DownloadableVersionConcept = std::is_base_of_v<package::DownloadableVersion, T>;

    template <DownloadableVersionConcept T>
    int fetch(const std::unordered_map<std::string, T> &resolved, std::ostream &out, const std::vector<std::string> &logWhenSeen, const std::filesystem::path &workingDirectory);

    template <DownloadableVersionConcept T>
    int fetch(const std::unordered_map<std::string, T> &resolved, std::ostream &out, const std::vector<std::string> &logWhenSeen) {
        return fetch(resolved, out, logWhenSeen, std::filesystem::current_path());
    }
}
