#include <doctest/doctest.h>

#include <sstream>

#include "tree/TreeRenderer.h"

using namespace config;
using namespace semver;
using namespace tree;

TEST_CASE("one direct dependency") {
    ManifestLock manifest{{{"foo", {Version(1, 0, 5), ""}}}};
    std::stringstream out;
    out << TreeRenderer{manifest, {"foo"}};
    CHECK_EQ(out.str(), "└─ foo v1.0.5\n");
}

TEST_CASE("multiple direct dependencies") {
    ManifestLock manifest{{
        {"foo", {Version(1, 0, 5), ""}},
        {"bar", {Version(2, 1, 7), ""}},
        {"baz", {Version(0, 4, 3), ""}},
    }};
    std::stringstream out;
    out << TreeRenderer{manifest, {"foo", "bar", "baz"}};
    CHECK_EQ(out.str(), "├─ baz v0.4.3\n"
                        "├─ bar v2.1.7\n"
                        "└─ foo v1.0.5\n");
}

TEST_CASE("nested dependencies") {
    ManifestLock manifest{{
        {"foo", {Version(1, 0, 5), "", {"bar"}}},
        {"bar", {Version(2, 1, 7), "", {"baz"}}},
        {"baz", {Version(0, 4, 3), ""}},
    }};
    std::stringstream out;
    out << TreeRenderer{manifest, {"foo"}};
    CHECK_EQ(out.str(), "└─ foo v1.0.5\n"
                        "   └─ bar v2.1.7\n"
                        "      └─ baz v0.4.3\n");
}

TEST_CASE("nested dependencies - continue line") {
    ManifestLock manifest{{
        {"foo", {Version(1, 0, 5), "", {"bar"}}},
        {"bar", {Version(2, 1, 7), ""}},
    }};
    std::stringstream out;
    out << TreeRenderer{manifest, {"bar", "foo"}};
    CHECK_EQ(out.str(), "├─ foo v1.0.5\n"
                        "│  └─ bar v2.1.7\n"
                        "└─ bar v2.1.7\n");
}
