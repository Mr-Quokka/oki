#include <doctest/doctest.h>
#include <sstream>

#include "semver/ParseException.h"
#include "semver/Version.h"

using namespace semver;

TEST_CASE("parsing valid versions") {
    CHECK(Version::parse("0.4.0") == Version(0, 4, 0));
    CHECK(Version::parse("1.0.0") == Version(1, 0, 0));
    CHECK(Version::parse("5.9.11") == Version(5, 9, 11));
}

TEST_CASE("parsing invalid versions") {
    CHECK_THROWS_AS(Version::parse("a.b.c"), ParseException);
    CHECK_THROWS_AS(Version::parse("5.2"), ParseException);
    CHECK_THROWS_AS(Version::parse("0.-1.6"), ParseException);
    CHECK_THROWS_AS(Version::parse("1.3.a5"), ParseException);
    CHECK_THROWS_AS(Version::parse("2 9 3"), ParseException);
    CHECK_THROWS_AS(Version::parse("4.2.3beta"), ParseException);
}

TEST_CASE("parsing valid offset") {
    try {
        Version::parse("372.21026.1824test");
        FAIL("no exception was thrown");
    } catch (const ParseException &exception) {
        CHECK_EQ(exception.getOffset(), 14);
    }
}

TEST_CASE("comparing versions") {
    CHECK((Version(1, 0, 4) <=> Version(1, 0, 3)) == std::strong_ordering::greater);
    CHECK((Version(0, 4, 2) <=> Version(0, 5, 0)) == std::strong_ordering::less);
    CHECK((Version(1, 10, 2) <=> Version(2, 4, 1)) == std::strong_ordering::less);
}

TEST_CASE("converting versions to string") {
    CHECK(Version(2, 0, 0).str() == "2.0.0");
    CHECK(Version(0, 1, 9).str() == "0.1.9");
    std::stringstream out;
    out << Version(3, 7, 1);
    CHECK(out.str() == "3.7.1");
}

TEST_CASE("getting max version") {
    CHECK_THROWS_AS(Version::maxVersion().nextAfter(), std::range_error);
}
