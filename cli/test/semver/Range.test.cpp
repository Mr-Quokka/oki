#include <doctest/doctest.h>

#include "semver/Range.h"

using namespace semver;

TEST_CASE("parsing valid ranges") {
    CHECK(Range::parse("*") == Range(Version(), Version::maxVersion()));
    CHECK(Range::parse("2.2.5") == Range(Version(2, 2, 5), Version(3, 0, 0)));
    CHECK(Range::parse("^1.2.5") == Range(Version(1, 2, 5), Version(2, 0, 0)));
    CHECK(Range::parse("~4.3.7") == Range(Version(4, 3, 7), Version(4, 4, 0)));
    CHECK(Range::parse("=3.0.6") == Range(Version(3, 0, 6), Version(3, 0, 7)));
}

TEST_CASE("contains") {
    Range range(Version(2, 1, 0), Version(3, 0, 0));
    CHECK(range.contains(Version(2, 1, 0)));
    CHECK(range.contains(Version(2, 5, 4)));
    CHECK(!range.contains(Version(3, 0, 0)));
}

TEST_CASE("intersection") {
    Range a(Version(1, 1, 1), Version(4, 4, 4));
    Range b(Version(2, 2, 2), Version(6, 6, 6));
    Range c(Version(2, 2, 2), Version(3, 3, 3));
    Range d(Version(5, 5, 5), Version(6, 6, 6));
    CHECK(a.intersection(a) == a);
    CHECK(a.intersection(b) == Range(Version(2, 2, 2), Version(4, 4, 4)));
    CHECK(a.intersection(c) == c);
    CHECK(a.intersection(d).isEmpty());
}
