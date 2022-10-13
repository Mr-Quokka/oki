#pragma once

#include <filesystem>

namespace oki{
    class Extractor {
    private:
        std::filesystem::path destination;
    public:
        explicit Extractor(std::filesystem::path destination);
        void extract(const std::filesystem::path &archivePath);
    };
}
