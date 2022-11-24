#pragma once

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

        static Range parse(std::string_view s);
    };
}
