#include <doctest/doctest.h>

#include "io/Checksum.h"

using namespace io;

TEST_CASE("from string") {
    const char *str = "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";
    Sha256Checksum checksum{str};
    CHECK(checksum.str() == str);
}
