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
        fs::path packageDependenciesDir = shared / packageName / OKI_PACKAGES_DIRECTORY;
        fs::create_directory(packageDependenciesDir);
        for (std::string_view dependency : graph.getTransitiveDependencies(packageName)) {
            fs::create_symlink(fs::relative(shared / dependency, packageDependenciesDir), packageDependenciesDir / dependency);
        }
        std::ofstream makefile{packageDependenciesDir / "Makefile"};
        makefile << "oki-built-dependencies:\n"
                    ".PHONY: oki-built-dependencies\n";
    }
}