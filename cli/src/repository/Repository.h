#pragma once

#include <filesystem>
#include <string_view>
#include <vector>

#include "../package/Package.h"

class Repository {
public:
    virtual std::vector<Package> listPackages() = 0;
    virtual void download(std::string_view packageName, const std::filesystem::path& destination) = 0;
    virtual ~Repository() = default;
};
