#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <string>

namespace io {
    using Sha256Checksum = std::array<unsigned char, 32>;
    using Checksum = Sha256Checksum;

    Sha256Checksum computeSha256(const std::filesystem::path &path);

    std::string checksumAsString(const Checksum &checksum);
}
