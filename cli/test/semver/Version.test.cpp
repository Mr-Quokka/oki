#include <doctest/doctest.h>

#include "semver/Version.h"

using namespace semver;

TEST_CASE("parsing valid versions") {
    CHECK(Version::parse("0.4.0") == Version(0, 4, 0));
    CHECK(Version::parse("1.0.0") == Version(1, 0, 0));
    CHECK(Version::parse("5.9.11") == Version(5, 9, 11));
}

TEST_CASE("comparing versions") {
    CHECK((Version(1, 0, 4) <=> Version(1, 0, 3)) == std::strong_ordering::greater);
    CHECK((Version(0, 4, 2) <=> Version(0, 5, 0)) == std::strong_ordering::less);
    CHECK((Version(1, 10, 2) <=> Version(2, 4, 1)) == std::strong_ordering::less);
}
