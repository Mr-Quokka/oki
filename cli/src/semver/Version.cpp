#include "Version.h"

#include "ParseException.h"
#include <charconv>

namespace semver {
    Version::Version(Value major, Value minor, Value patch)
        : major{major}, minor{minor}, patch{patch} {}

    Version Version::nextAfter() const {
        Version ret = *this;
        if (ret.patch != MaxValue) {
            ++ret.patch;
        } else {
            ret.patch = 0;
            if (ret.minor != MaxValue) {
                ++ret.minor;
            } else {
                ret.minor = 0;
                if (ret.major != MaxValue) {
                    ++ret.major;
                } else {
                    throw std::range_error{"Cannot get nextAfter() of maxVersion()"};
                }
            }
        }
        return ret;
    }

    std::string Version::str() const {
        return operator std::string();
    }

    Version::operator std::string() const {
        std::string buffer;
        buffer.append(std::to_string(major));
        buffer.append(".");
        buffer.append(std::to_string(minor));
        buffer.append(".");
        buffer.append(std::to_string(patch));
        return buffer;
    }

    Version Version::parse(std::string_view s) {
        return parseWithContext(s, s);
    }

    Version Version::parseWithContext(std::string_view s, std::string_view ctx) {
        const char *ptr = s.data();
        const char *begin = ctx.begin();
        const char *end = s.cend();
        Version ret{};

        std::from_chars_result result = std::from_chars(ptr, end, ret.major);
        auto isError = [&]() { return result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range; };
        auto isEnd = [&]() { return result.ptr == end || *result.ptr != '.'; };
        if (isError() || isEnd()) {
            throw ParseException{"Invalid major number", ctx, ptr - begin};
        }

        ptr = result.ptr + 1;
        result = std::from_chars(ptr, end, ret.minor);
        if (isError() || isEnd()) {
            throw ParseException{"Invalid minor number", ctx, ptr - begin};
        }

        ptr = result.ptr + 1;
        result = std::from_chars(ptr, end, ret.patch);
        if (isError()) {
            throw ParseException{"Invalid patch number", ctx, ptr - begin};
        }
        if (result.ptr != end) {
            throw ParseException{"Unhandled version qualifier", ctx, result.ptr - begin};
        }
        return ret;
    }

    std::ostream &operator<<(std::ostream &os, const Version &version) {
        return os << version.major << "." << version.minor << "." << version.patch;
    }
}
