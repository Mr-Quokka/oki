#include "Range.h"
#include <sstream>

#include "ParseException.h"

namespace semver {
    Range::Range(const Version &exact)
        : minInclusive{exact}, maxExclusive{exact.nextAfter()} {}

    Range::Range(const Version &minInclusive, const Version &maxExclusive)
        : minInclusive{minInclusive}, maxExclusive{maxExclusive} {
        if (minInclusive > maxExclusive) {
            throw std::range_error{"Invalid range"};
        }
    }

    bool Range::contains(const Version &version) const {
        return minInclusive <= version && version < maxExclusive;
    }

    Range Range::intersection(const Range &other) const {
        const Version &min = std::max(minInclusive, other.minInclusive);
        const Version &max = std::max(min, std::min(maxExclusive, other.maxExclusive));
        return {min, max};
    }

    bool Range::operator==(const Range &other) const {
        return minInclusive == other.minInclusive && maxExclusive == other.maxExclusive;
    }

    bool Range::operator!=(const Range &other) const {
        return !(*this == other);
    }

    bool Range::isEmpty() const {
        return minInclusive == maxExclusive;
    }

    std::string Range::str() const {
        std::ostringstream oss;
        oss << *this;
        return oss.str();
    }

    Range Range::parse(std::string_view s) {
        if (s == "*") {
            return {Version{}, Version::maxVersion()};
        }
        const char *ptr = s.data();
        const auto stop = ptr + s.size();
        if (*ptr == '=') {
            ++ptr;
            return Range{Version::parseWithContext(std::string_view{ptr, static_cast<std::string_view::size_type>(stop - ptr)}, s)};
        } else if (*ptr == '~') {
            ++ptr;
            Version ver = Version::parseWithContext(std::string_view{ptr, static_cast<std::string_view::size_type>(stop - ptr)}, s);
            Version v2 = ver;
            v2.patch = Version::MaxValue;
            return {ver, v2.nextAfter()};
        } else if (*ptr == '^' || std::isdigit(*ptr)) {
            if (*ptr == '^') {
                ++ptr;
            }
            Version ver = Version::parseWithContext(std::string_view{ptr, static_cast<std::string_view::size_type>(stop - ptr)}, s);
            Version v2 = ver;
            v2.patch = Version::MaxValue;
            v2.minor = Version::MaxValue;
            return {ver, v2.nextAfter()};
        } else {
            throw ParseException{"Unknown version requirement", s, 0L};
        }
    }

    Range Range::featureSatisfying(const Version &version) {
        Version v2 = version;
        v2.minor = Version::MaxValue;
        v2.patch = Version::MaxValue;
        return {version, v2.nextAfter()};
    }

    std::ostream &operator<<(std::ostream &os, const Range &range) {
        if (range.minInclusive.nextAfter() == range.maxExclusive) {
            os << range.minInclusive;
        } else if (range.minInclusive == Version{} && range.maxExclusive == Version::maxVersion()) {
            os << "*";
        } else if (range.maxExclusive.major == (range.minInclusive.major + 1) && range.maxExclusive.minor == 0 && range.maxExclusive.patch == 0) {
            os << range.minInclusive;
        } else if (range.maxExclusive.major == range.minInclusive.major && range.maxExclusive.minor == (range.minInclusive.minor + 1) && range.maxExclusive.patch == 0) {
            os << "~" << range.minInclusive;
        } else {
            os << ">=" << range.minInclusive << ", <" << range.maxExclusive;
        }
        return os;
    }
}
