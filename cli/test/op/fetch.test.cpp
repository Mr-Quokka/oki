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
    fetch(config::ManifestLock{}, nullStream, {{}, true}, tmp.path());
}

TEST_CASE("fetch one dependency") {
    io::TmpDir tmpDir;
    io::TmpFile dependencyFile;
    Compressor compressor{dependencyFile.path()};
    compressor.compress(tmpDir.path());

    std::string url = "file:///";
    url += dependencyFile.path();

    std::stringstream out;
    CHECK_EQ(fetch(config::ManifestLock{std::unordered_map<std::string, package::VersionLock>{
                       {"foo", package::VersionLock{Version{5, 2, 3}, url}}}},
                   out, {{}, true}, tmpDir.path()),
             0);
    CHECK_EQ(out.str(), "Installed 1 package\n");
}
