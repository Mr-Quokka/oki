#include "MakefileAction.h"
#include "../config/ManifestLock.h"
#include "../io/oki.h"
#include "../make/BuildConfigurer.h"
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

        // Cree un fichier makefile
        std::cout << "Creating Makefile : " << fileName << "\n";
        std::ofstream makefile{std::string{fileName}};
        if (!makefile) {
            exit(1);
        }

        // Debut du makefile
        makefile << "CC := gcc\n"
                    "CFLAGS := -std=c17 -Wall -Wextra -pedantic -g -MMD -MP\n"
                    "\nTARGET_EXE := "
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
        makefile << "\nsources := $(wildcard $(SRC_DIR)/*.c)\n"
                    "objets := $(sources:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)\n"
                    "dependances := $(objets:.o=.d)\n"
                    "\n$(TARGET_EXE): $(objets) | oki-built-dependencies\n"
                    "\t$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)\n"
                    "\n$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)\n"
                    "\t$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@\n"
                    "\n$(BUILD_DIR):\n"
                    "\tmkdir $(BUILD_DIR)\n"
                    "\n.PHONY: clean oki-built-dependencies\n"
                    "clean:\n"
                    "\trm -rf $(BUILD_DIR) $(TARGET_EXE)\n"
                    "oki-built-dependencies:\n"
                    "\toki build\n"
                    "\n-include $(dependances)\n";

        std::ofstream internalMakefile{OKI_INTERNAL_MAKEFILE};
        depBuild.asMakefile(internalMakefile);
    }
}
