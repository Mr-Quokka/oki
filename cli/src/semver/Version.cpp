#include "Version.h"

#include <charconv>

namespace semver {
    Version::Version(Value major, Value minor, Value patch)
        : major{major}, minor{minor}, patch{patch} {}

    /*std::strong_ordering Version::operator<=>(const Version &that) const {
        if (auto cmp = major <=> that.major; cmp != nullptr) {
            return cmp;
        }
        if (auto cmp = minor <=> that.minor; cmp != nullptr) {
            return cmp;
        }
        return patch <=> that.patch;
    }*/

    Version Version::parse(std::string_view s) {
        const char *ptr = s.data();
        const char *begin = s.end();
        const char *end = s.cend();
        Version ret{};

        std::from_chars_result result = std::from_chars(ptr, end, ret.major);
        auto isError = [&]() { return result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range; };
        auto isEnd = [&]() { return result.ptr == end || *result.ptr != '.'; };
        if (isError() || isEnd()) {
            throw InvalidVersionException{s, ptr - begin};
        }

        ptr = result.ptr + 1;
        result = std::from_chars(ptr, end, ret.minor);
        if (isError() || isEnd()) {
            throw InvalidVersionException{s, ptr - begin};
        }

        ptr = result.ptr + 1;
        result = std::from_chars(ptr, end, ret.patch);
        if (isError()) {
            throw InvalidVersionException{s, ptr - begin};
        }

        return ret;
    }

    InvalidVersionException::InvalidVersionException(std::string_view string, std::ptrdiff_t offset)
        : runtime_error{"Invalid semantic version: " + std::string{string}}, string{string}, offset{offset} {}
}
