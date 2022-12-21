#include <doctest/doctest.h>

#include "io/TmpFile.h"
#include "op/fetch.h"
#include "package/VersionLock.h"
#include <fstream>
#include <iostream>

using namespace op;

TEST_CASE("fetch empty") {
    io::TmpDir tmp;
    std::ofstream nullStream;
    nullStream.setstate(std::ios_base::badbit);
    fetch(std::unordered_map<std::string, package::VersionLock>{}, nullStream, {}, tmp.getDirname());
}
