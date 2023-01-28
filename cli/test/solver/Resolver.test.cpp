#include <algorithm>
#include <doctest/doctest.h>
#include <vector>

#include "solver/Resolver.h"

using namespace package;
using namespace semver;
using namespace solver;

static PackageVersion pkg(const semver::Version &version, const Summaries &summaries = {}) {
    return {version, "", "", summaries};
}

class RepositoryMock : public repository::Repository {
public:
    std::vector<package::Package> listPackages() override {
        return {};
    }
    package::Package getPackageInfo(std::string_view packageName) override {
        if (packageName == "foo") {
            return {"foo", "", {pkg(Version{2, 0, 0}), pkg(Version{1, 2, 0}), pkg(Version{1, 1, 1}), pkg(Version{1, 1, 0}), pkg(Version{1, 0, 0}), pkg(Version{0, 1, 0})}};
        }
        if (packageName == "bar") {
            return {"bar", "", {pkg(Version{3, 0, 0}), pkg(Version{2, 4, 0}), pkg(Version{2, 0, 1}), pkg(Version{1, 0, 0})}};
        }
        if (packageName == "baz") {
            return {"baz", "", {pkg(Version{1, 5, 0}, {{"foo", Range::parse("^1.0.1")}}), pkg(Version{1, 4, 0}, {{"foo", Range::parse("^1.0.0")}})}};
        }
        if (packageName == "fred") {
            return {"fred", "", {pkg(Version{1, 0, 0}, {{"foo", Range::parse("^1.1.1")}})}};
        }
        if (packageName == "grault") {
            return {"grault", "", {pkg(Version{1, 0, 0}, {{"baz", Range::parse("*")}, {"waldo", Range::parse("*")}}), pkg(Version{0, 1, 0}, {{"baz", Range::parse("*")}, {"waldo", Range::parse("2.0.0")}})}};
        }
        if (packageName == "waldo") {
            return {"waldo", "", {pkg(Version{2, 0, 0}, {{"foo", Range::parse("^2.0.0")}}), pkg(Version{1, 5, 1}, {{"foo", Range::parse("~1.1.0")}})}};
        }
        throw std::logic_error{"Not implemented"};
    }
    bool publish([[maybe_unused]] config::Manifest &manifest, [[maybe_unused]] const std::filesystem::path &source) override {
        return true;
    }
};

TEST_CASE("no indirect dependency - optimal solution") {
    Summaries summaries{
        {"foo", Range::parse("^1.0.0")},
        {"bar", Range::parse("~2.0.0")},
    };
    RepositoryMock mock;
    Resolved resolved = resolve(summaries, mock);
    CHECK_EQ(resolved.size(), 2);
    CHECK_EQ(resolved.at("foo"), Version(1, 2, 0));
    CHECK_EQ(resolved.at("bar"), Version(2, 0, 1));
}

TEST_CASE("no indirect dependency - no solution") {
    Summaries summaries{
        {"foo", Range::parse("^1.0.0")},
        {"bar", Range::parse("^2.5.0")},
    };
    RepositoryMock mock;
    CHECK_THROWS_WITH(resolve(summaries, mock), "no matching version found for package bar");
}

TEST_CASE("diamond dependency - optimal solution") {
    Summaries summaries{
        {"grault", Range::parse("^1.0.0")},
    };
    RepositoryMock mock;
    Resolved resolved = resolve(summaries, mock);
    CHECK_EQ(resolved.size(), 4);
    CHECK_EQ(resolved.at("grault"), Version(1, 0, 0));
    CHECK_EQ(resolved.at("baz"), Version(1, 5, 0));
    CHECK_EQ(resolved.at("waldo"), Version(1, 5, 1));
    CHECK_EQ(resolved.at("foo"), Version(1, 1, 1));
}

TEST_CASE("diamond dependency - no solution") {
    Summaries summaries{
        {"grault", Range::parse("~0.1.0")},
    };
    RepositoryMock mock;
    CHECK_THROWS_WITH(resolve(summaries, mock), "no matching version found for package foo");
}

TEST_CASE("diamond dependency - version intersection") {
    std::string fred = "fred";
    std::string waldo = "waldo";
    std::vector<Dependency> dependencies{
        {fred, Range::parse("=1.0.0")},
        {waldo, Range::parse("=1.5.1")},
    };
    RepositoryMock mock;
    Resolved resolved = resolve(dependencies, mock);

    // Vérification que l'ordre de déclaration des dépendances n'est pas déterministe
    std::reverse(dependencies.begin(), dependencies.end());
    CHECK_EQ(resolved, resolve(dependencies, mock));

    CHECK_EQ(resolved.size(), 3);
    CHECK_EQ(resolved.at("fred"), Version(1, 0, 0));
    CHECK_EQ(resolved.at("waldo"), Version(1, 5, 1));
    CHECK_EQ(resolved.at("foo"), Version(1, 1, 1));
}
