#pragma once

#include <algorithm>

#include "Version.h"

namespace semver {
    class Range {
    private:
        Version minInclusive;
        Version maxExclusive;

    public:
        explicit Range(const Version &exact);
        Range(const Version &minInclusive, const Version &maxExclusive);
        bool contains(const Version &version) const;
        Range intersection(const Range &other) const;
        bool isEmpty() const;
        bool operator==(const Range &other) const;
        bool operator!=(const Range &other) const;

        template <std::forward_iterator Iter, std::sentinel_for<Iter> Stop>
        Iter findSatisfying(Iter begin, const Stop end) const {
            return std::find_if(begin, end, [&](const Version &version) { return contains(version); });
        }

        static Range parse(std::string_view s);
    };
}
