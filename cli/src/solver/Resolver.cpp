#include "Resolver.h"

#include <algorithm>

namespace solver {
    void Resolver::fetchVersions(const Summaries &summaries, repository::Repository &repository) {
        for (const auto &[pkg, requirement] : summaries) {
            auto existing = registry.find(pkg);
            if (existing == registry.end()) {
                package::Package package = repository.getPackageInfo(pkg);
                registry.insert(std::make_pair(pkg, std::move(package)));
            }
        }
    }

    Resolved Resolver::resolve(const Summaries &summaries, repository::Repository &repository) {
        fetchVersions(summaries, repository);
        Resolved resolved;
        std::transform(summaries.cbegin(), summaries.cend(), std::inserter(resolved, resolved.end()), [&](const auto &pair) {
            const std::vector<package::PackageVersion> versions = registry.find(pair.first)->second.getVersions();
            std::forward_iterator auto it = pair.second.findSatisfying(versions.cbegin(), versions.cend());
            if (it == versions.cend()) {
                throw std::range_error{"no matching version found for package " + pair.first};
            }
            return std::make_pair(pair.first, *it);
        });
        return resolved;
    }

    Resolved resolve(const solver::Summaries &summaries, repository::Repository &repository) {
        return Resolver{}.resolve(summaries, repository);
    }
}
