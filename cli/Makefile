# Arguments pour la compilation
CXX := g++
CXXFLAGS := -std=c++20 \
	-Wall -Wextra \
	-Wno-redundant-move \
	-Wnon-virtual-dtor \
	-Wsuggest-override \
	-pedantic \
	-g -MMD -MP \
	-fdiagnostics-color=always
LDLIBS := -lcurl # Libraries utilisées pour l'édition des liens

# Nom de l'exécutable final
TARGET_EXE := oki 

# Répertoires des fichiers compilés et de sources
BUILD_DIR := build
SRC_DIR := src src/cli src/config src/io src/package src/repository

# Répertoires où rechercher les fichiers header
CPPFLAGS=-Ithird-party

# Liste tous les fichiers sources .cpp
sources := $(foreach dir,$(SRC_DIR),$(wildcard $(dir)/*.cpp))
# Déduit tous les fichiers objets .o à partir du nom des fichiers sources
objets := $(sources:src/%.cpp=$(BUILD_DIR)/%.o)
# Déduit tous les fichiers dépendances .o à partir des fichiers objets
dependances := $(objets:.o=.d)

all: $(TARGET_EXE)

# Créé l'exécutable final avec tous les fichiers objets
$(TARGET_EXE): $(objets)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Définit la compilation des fichiers C++
# Le dossier de build est créé s'il n'existe pas encore
$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Créé le répertoire de build
$(BUILD_DIR):
	mkdir -p $(subst src,$(BUILD_DIR),$(SRC_DIR))

# Supprime tous les fichiers issus de la compilation
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET_EXE)

# Inclut les Makefiles de chaque fichier source
# Le - permet d'ignorer les Makefiles manquants
-include $(dependances)
