#include "BuildConfigurer.h"
#include "../io/oki.h"
#include <fstream>

namespace fs = std::filesystem;

namespace make {
    BuildConfigurer::BuildConfigurer(solver::DependencyGraph graph, std::filesystem::path &shared)
        : graph{graph}, shared{shared} {}

    void BuildConfigurer::configure(const std::string &packageName) {
        linkLocalDependencies(packageName);
    }

    void BuildConfigurer::linkLocalDependencies(const std::string &packageName) {
        std::vector<std::string_view> dependencies = graph.getTransitiveDependencies(packageName);
        if (dependencies.empty()) {
            return;
        }
        fs::path packageDependenciesDir = shared / packageName / OKI_PACKAGES_DIRECTORY;
        fs::create_directory(packageDependenciesDir);
        for (std::string_view dependency : dependencies) {
            fs::create_symlink(fs::relative(shared / dependency, packageDependenciesDir), packageDependenciesDir / dependency);
        }
        std::ofstream makefile{packageDependenciesDir / "Makefile"};
        makefile << "oki-built-dependencies:\n"
                    ".PHONY: oki-built-dependencies\n";
    }

    void BuildConfigurer::gatherBuildInfoForRoot(DependencyBuild &depBuild, std::vector<std::string> &exports, std::vector<std::string> &staticLink) {
        // L'ordre est important puisque les dépendances les plus profondes doivent être liées en dernier
        std::vector<std::string> dependencies = graph.topologicalSort();
        for (const std::string &dependency : dependencies) {
            fs::path dependencyPath = shared / dependency;
            config::Manifest manifest = config::Manifest::fromFile(dependencyPath / OKI_MANIFEST_FILE);
            make::BuildInfo build = manifest.getBuildInfo();
            if (build.buildStatic.has_value() && build.staticLink.has_value()) {
                std::string target = (dependencyPath / build.staticLink.value()).string();
                depBuild.addBuildStep(target, "cd " + dependencyPath.string() + " && " + build.buildStatic.value());
                staticLink.emplace_back(target);
            }
            std::transform(
                build.exports.cbegin(),
                build.exports.cend(),
                std::back_inserter(exports),
                [&dependencyPath](const std::string &exported) {
                    return (dependencyPath / exported).string();
                });
        }
    }
}