#include "MakefileAction.h"
#include "../config/ManifestLock.h"
#include "../io/oki.h"
#include "../make/BuildConfigurer.h"
#include "../make/CCompilatorStrategy.h"
#include "../make/CompilatorStrategy.h"
#include "../make/CppCompilatorStrategy.h"
#include "../solver/DependencyGraph.h"
#include "../util/ostreamJoin.h"
#include "ExitStatuses.h"

#include <fstream>
#include <iostream>
#include <vector>

namespace cli {
    MakefileAction::MakefileAction(config::UserConfig &config, ArgMatches &&args)
        : config{config}, fileName{args.get<std::string>("file").value_or("Makefile")} {}

    int MakefileAction::run() {
        config::Manifest manifest = config::Manifest::fromFile(OKI_MANIFEST_FILE);
        config::ManifestLock manifestLock = config::ManifestLock::readOrResolve(OKI_MANIFEST_FILE, OKI_LOCK_FILE, config.getGlobalRepository());

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
            return ERR_CANT_CREATE;
        }

        // Debut du makefile
        strategy->writeMakefileStart(makefile);
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
        strategy->writeMakefileEnd(makefile);
        std::ofstream internalMakefile{OKI_INTERNAL_MAKEFILE};
        depBuild.asMakefile(internalMakefile);
        return OK;
    }

    Command MakefileAction::cmd() {
        return Command{"makefile", "Generate a makefile",
                       [](config::UserConfig &conf, ArgMatches &&args) -> std::unique_ptr<CliAction> {
                           return std::make_unique<MakefileAction>(conf, std::move(args));
                       }}
            .arg<std::string>("file", "Name of the makefile to generate")
            .positional("file");
    }
}
