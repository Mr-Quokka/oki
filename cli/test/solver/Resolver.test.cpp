#include <doctest/doctest.h>
#include <vector>

#include "solver/Resolver.h"

using namespace semver;
using namespace solver;

class RepositoryMock : public repository::Repository {
public:
    std::vector<package::Package> listPackages() override {
        return {};
    }
    package::Package getPackageInfo(std::string_view packageName) override {
        if (packageName == "foo") {
            return {"foo", "", {{Version{2, 0, 0}, "", ""}, {Version{1, 2, 0}, "", ""}, {Version{1, 1, 1}, "", ""}, {Version{1, 1, 0}, "", ""}, {Version{1, 0, 0}, "", ""}, {Version{0, 1, 0}, "", ""}}};
        }
        if (packageName == "bar") {
            return {"bar", "", {{Version{3, 0, 0}, "", ""}, {Version{2, 4, 0}, "", ""}, {Version{2, 0, 1}, "", ""}, {Version{1, 0, 0}, "", ""}}};
        }
        throw std::logic_error{"Not implemented"};
    }
    std::string getPackageURL(std::string_view packageName, std::string packageVersion) override {
        return "";
    }
    void download(const package::PackageVersion &packageVersion, const std::filesystem::path &destination) override {
    }
};

TEST_CASE("no indirect dependency - optimal solution") {
    Summaries summaries{
        {"foo", Range::parse("^1.0.0")},
        {"bar", Range::parse("~2.0.0")},
    };
    RepositoryMock mock;
    Resolver resolver;
    resolver.resolve(summaries, mock);
}

TEST_CASE("no indirect dependency - no solution") {
    Summaries summaries{
        {"foo", Range::parse("^1.0.0")},
        {"bar", Range::parse("^2.5.0")},
    };
    RepositoryMock mock;
    CHECK_THROWS_WITH(resolve(summaries, mock), "no matching version found for package bar");
}
