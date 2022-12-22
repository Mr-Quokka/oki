#include <doctest/doctest.h>
#include <sstream>

#include "io/Archive.h"
#include "io/TmpFile.h"
#include "op/fetch.h"
#include "package/VersionLock.h"
#include <fstream>
#include <iostream>

using namespace io;
using namespace semver;
using namespace op;

TEST_CASE("fetch empty") {
    io::TmpDir tmp;
    std::ofstream nullStream;
    nullStream.setstate(std::ios_base::badbit);
    fetch(std::unordered_map<std::string, package::VersionLock>{}, nullStream, {}, tmp.getDirname());
}

TEST_CASE("fetch one dependency") {
    io::TmpDir tmpDir;
    io::TmpFile dependencyFile;
    Compressor compressor{dependencyFile.getFilename()};
    compressor.compress(tmpDir.getDirname());

    std::string url = "file:///";
    url += dependencyFile.getFilename();

    std::stringstream out;
    CHECK_EQ(fetch(std::unordered_map<std::string, package::VersionLock>{
                       {"foo", package::VersionLock{Version{5, 2, 3}, url}}},
                   out, {}, tmpDir.getDirname()),
             0);
    CHECK_EQ(out.str(), "Installed 1 package\n");
}
