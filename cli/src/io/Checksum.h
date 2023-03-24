#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <string>

namespace io {
    class Sha256Checksum : public std::array<uint8_t, 32> {
    public:
        Sha256Checksum() = default;
        explicit Sha256Checksum(const std::array<uint8_t, 32> &array);
        explicit Sha256Checksum(std::string_view str);
        std::string str() const;

        static Sha256Checksum ofFile(const std::filesystem::path &path);
    };
    std::ostream &operator<<(std::ostream &out, const Sha256Checksum &checksum);
    using Checksum = Sha256Checksum;
}
