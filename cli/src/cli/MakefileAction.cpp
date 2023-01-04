#include "MakefileAction.h"
#include "../config/ManifestLock.h"
#include "../io/oki.h"
#include "../make/BuildConfigurer.h"
#include "../make/CCompilatorStrategy.h"
#include "../make/CompilatorStrategy.h"
#include "../make/CppCompilatorStrategy.h"
#include "../solver/DependencyGraph.h"
#include "../util/ostreamJoin.h"

#include <fstream>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

namespace cli {
    MakefileAction::MakefileAction(const char *fileName) : fileName{fileName} {}

    void MakefileAction::run(repository::Repository &repository) {
        config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
        config::ManifestLock manifestLock = config::ManifestLock::readOrResolve(OKI_MANIFEST_FILE, OKI_LOCK_FILE, repository);

        std::unique_ptr<make::CompilatorStrategy> strategy;

        switch (manifest.getProjectKind()) {
        case make::ProjectKind::C:
            strategy = std::make_unique<make::CCompilatorStrategy>();
            break;
        case make::ProjectKind::Cpp:
            strategy = std::make_unique<make::CppCompilatorStrategy>();
            break;
        default:
            throw std::range_error("Unknown project kind");
        }

        // Cree un fichier makefile
        std::cout << "Creating Makefile : " << fileName << "\n";
        std::ofstream makefile{std::string{fileName}};
        if (!makefile) {
            exit(1);
        }

        // Debut du makefile
        strategy->writeStart(makefile);
        makefile << "\nTARGET_EXE := "
                 << manifest.getProjectName() << "\n";
        makefile << "\nBUILD_DIR := build\n"
                    "SRC_DIR := src\n\n";

        // Collecte les informations de construction
        make::DependencyBuild depBuild;
        make::BuildConfigurer configurer{solver::DependencyGraph{manifestLock}, OKI_PACKAGES_DIRECTORY};
        std::vector<std::string> exposes;
        std::vector<std::string> staticLinks;
        configurer.gatherBuildInfoForRoot(depBuild, exposes, staticLinks);

        // Ajoute les dépendances de fichiers header
        makefile << "CPPFLAGS := ";
        util::ostreamJoin(makefile, exposes.cbegin(), exposes.cend(), " \\\n\t-isystem", "-isystem");
        makefile << "\n";

        // Ajoute les dépendances de bibliothèques statiques
        makefile << "LDLIBS := ";
        util::ostreamJoin(makefile, staticLinks.cbegin(), staticLinks.cend(), " \\\n\t");
        makefile << "\n";

        // Suite du makefile
        strategy->writeEnd(makefile);
        std::ofstream internalMakefile{OKI_INTERNAL_MAKEFILE};
        depBuild.asMakefile(internalMakefile);
    }
}
