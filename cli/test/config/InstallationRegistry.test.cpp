#include <doctest/doctest.h>

#include "config/InstallationRegistry.h"
#include "io/TmpFile.h"
#include "io/oki.h"
#include <fstream>

namespace fs = std::filesystem;

using namespace config;

TEST_CASE("emplace") {
    InstallationRegistry registry;
    package::DownloadableVersion downloadable{semver::Version(1, 0, 0), ""};
    auto [it, inserted] = registry.emplace("foo", downloadable);
    CHECK(inserted);
    CHECK(it->second == downloadable);
    CHECK(registry.containsExact("foo", downloadable));

    package::DownloadableVersion downloadable2{semver::Version(2, 0, 0), ""};
    auto [it2, inserted2] = registry.emplace("foo", downloadable);
    CHECK_FALSE(inserted2);
    CHECK(it2->second == downloadable);
    CHECK(registry.containsExact("foo", downloadable));

    auto iterator = registry.cbegin();
    CHECK_NE(iterator, registry.cend());
    CHECK_EQ(iterator->second, it->second);
    ++iterator;
    CHECK_EQ(iterator, registry.cend());
}

TEST_CASE("erase") {
    InstallationRegistry registry;
    CHECK_FALSE(registry.erase("foo"));
    CHECK_FALSE(registry.erase("bar"));

    package::DownloadableVersion downloadable{semver::Version(1, 0, 0), ""};
    registry.emplace("foo", downloadable);
    CHECK(registry.erase("foo"));
    CHECK_FALSE(registry.erase("foo"));
    CHECK_FALSE(registry.erase("bar"));
}

TEST_CASE("round trip") {
    io::TmpFile tmp;
    InstallationRegistry registry = InstallationRegistry::loadFileIfExists(tmp.path());

    package::DownloadableVersion downloadable{semver::Version(5, 4, 1), ""};
    registry.emplace("baz", downloadable);
    registry.saveToFile(tmp.path());
    registry = InstallationRegistry::loadFileIfExists(tmp.path());
    CHECK(registry.containsExact("baz", downloadable));
}
