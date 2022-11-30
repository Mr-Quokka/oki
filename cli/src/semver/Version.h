#pragma once

#include <array>
#include <compare>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string_view>

namespace semver {
    /**
     * https://semver.org/lang/fr
     */
    struct Version {
        using Value = unsigned int;
        Value major, minor, patch;
        static constexpr Value MaxValue = std::numeric_limits<Value>::max();

        Version() = default;
        Version(Value major, Value minor, Value patch);
        Version nextAfter() const;
        std::string str() const;
        std::strong_ordering operator<=>(const Version &) const = default;
        friend std::ostream &operator<<(std::ostream &out, const Version &);
        operator std::string() const;

        static Version parse(std::string_view s);
        static Version parseWithContext(std::string_view s, std::string_view ctx);

        static Version maxVersion() {
            return {MaxValue, MaxValue, MaxValue};
        }
    };

    std::ostream &operator<<(std::ostream &out, const Version &version);
}
