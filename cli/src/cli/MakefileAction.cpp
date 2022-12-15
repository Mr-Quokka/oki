#include "MakefileAction.h"
#include "../config/ManifestLock.h"
#include "../io/oki.h"

#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

namespace cli {
    MakefileAction::MakefileAction(const char *fileName) : fileName{fileName} {}

    void MakefileAction::run(repository::Repository &repository) {
        config::ManifestLock manifestLock = config::ManifestLock::fromFile(OKI_LOCK_FILE);

        // Cree un fichier makefile
        std::cout << "Creation du fichier makefile : " << fileName << "\n";
        std::ofstream makefile(std::string(fileName).c_str());

        // Debut du makefile
        makefile << "CC := gcc\n"
                    "CFLAGS := -std=c17 -Wall -Wextra -pedantic -g -MMD -MP\n"
                    "\nTARGET_EXE := exe\n"
                    "\nBUILD_DIR := build\n"
                    "SRC_DIR := src\n"
                    "\nCPPFLAGS= ";

        // Ajoute les dependances
        for (auto it = manifestLock.getLocks().begin(); it != manifestLock.getLocks().end(); it++) {
            std::cout << "Ajoute la dependance au fichier : " << it->first << "\n";
            if (it->first == manifestLock.getLocks().begin()->first) {
                makefile << "-I " << it->first;
            } else {
                makefile << " \\\n\t -I " << it->first;
            }
        }

        // Suite du makefile
        makefile << "\n\nsources := $(wildcard $(SRC_DIR)/*.c)\n"
                    "objets := $(sources:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)\n"
                    "dependances := $(objets:.o=.d)\n"
                    "\n$(TARGET_EXE): $(objets)\n"
                    "\t$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)\n"
                    "\n$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)\n"
                    "\t$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@\n"
                    "\n$(BUILD_DIR):\n"
                    "\tmkdir $(BUILD_DIR)\n"
                    "\n.PHONY: clean\n"
                    "clean:\n"
                    "\trm -rf $(BUILD_DIR) $(TARGET_EXE)\n"
                    "\n-include $(dependances)\n";

        makefile.close();
    }
}