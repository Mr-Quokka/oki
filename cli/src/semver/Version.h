#pragma once

#include <array>
#include <compare>
#include <stdexcept>
#include <string_view>

namespace semver {
    /**
     * https://semver.org/lang/fr
     */
    class Version {
    private:
        using Value = unsigned int;
        Value major, minor, patch;

    public:
        Version() = default;
        Version(Value major, Value minor, Value patch);
        std::strong_ordering operator<=>(const Version &) const = default;

        static Version parse(std::string_view s);
    };

    class InvalidVersionException : std::runtime_error {
    private:
        std::string string;
        std::ptrdiff_t offset = 0;

    public:
        InvalidVersionException(std::string_view string, std::ptrdiff_t offset);
    };
}
