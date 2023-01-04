#include "BuildAction.h"
#include "../config/ManifestLock.h"
#include "../io/oki.h"
#include "../make/BuildConfigurer.h"
#include "../make/DependencyBuild.h"
#include "../solver/DependencyGraph.h"
#include <fstream>
#include <unistd.h>

namespace fs = std::filesystem;

namespace cli {
    void BuildAction::run(repository::Repository &repository) {
        bootstrap(repository);
        execlp("make", "make", "-f", OKI_INTERNAL_MAKEFILE.c_str(), nullptr);
    }

    void BuildAction::bootstrap(repository::Repository &repository) {
        std::error_code error;
        fs::file_time_type makeTime = fs::last_write_time(OKI_INTERNAL_MAKEFILE, error);
        fs::file_time_type lockTime = fs::last_write_time(OKI_LOCK_FILE, error);
        fs::file_time_type registryTime = fs::last_write_time(OKI_INTERNAL_REGISTRY_FILE, error);
        if (fs::last_write_time(OKI_MANIFEST_FILE) <= lockTime && lockTime <= makeTime && registryTime <= makeTime) {
            return;
        }
        config::ManifestLock manifestLock = config::ManifestLock::readOrResolve(OKI_MANIFEST_FILE, OKI_LOCK_FILE, repository);
        make::DependencyBuild depBuild;
        make::BuildConfigurer configurer{solver::DependencyGraph{manifestLock}, OKI_PACKAGES_DIRECTORY};
        std::vector<std::string> exposes;
        std::vector<std::string> staticLinks;
        configurer.gatherBuildInfoForRoot(depBuild, exposes, staticLinks);
        fs::create_directories(OKI_PACKAGES_DIRECTORY);
        std::ofstream internalMakefile{OKI_INTERNAL_MAKEFILE};
        depBuild.asMakefile(internalMakefile);
    }
}
