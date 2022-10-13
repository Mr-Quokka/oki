#pragma once

#include <filesystem>
#include <string_view>
#include <vector>
#include <optional>

#include "../package/Package.h"

namespace oki{
    class Repository {
    public:
        virtual std::vector<Package> listPackages() = 0;
        virtual std::optional<Package> showPackage(std::string_view packageName) = 0;
        virtual void download(const Version &packageVersion, const std::filesystem::path& destination) = 0;
        virtual ~Repository() = default;
    };
}
