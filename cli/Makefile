# Profil par défaut
BUILD := debug

# Arguments pour la compilation
CXX := g++
CXXFLAGS_debug := -g -DDEBUG
CXXFLAGS_test := -fprofile-arcs -ftest-coverage
CXXFLAGS_release := -O3
CXXFLAGS := -std=c++20 \
	-Wall -Wextra \
	-Wno-redundant-move \
	-Wnon-virtual-dtor \
	-Wsuggest-override \
	-Wconversion \
	-pedantic \
	-MMD -MP \
	$(CXXFLAGS_$(BUILD))
LDLIBS := -lcurl -lminizip # Bibliothèques utilisées pour l'édition des liens

# Nom de l'exécutable final
TARGET_EXE := oki

# Répertoires des fichiers compilés et de sources
BUILD_DIR := build/$(BUILD)
SRC_DIR := src src/cli src/config src/io src/package src/repository src/semver
TEST_DIR := test test/semver

# Répertoires où rechercher les fichiers header
CPPFLAGS=-isystemthird-party -Isrc

# Liste tous les fichiers sources .cpp
sources := $(foreach dir,$(SRC_DIR),$(wildcard $(dir)/*.cpp))
tests := $(foreach dir,$(TEST_DIR),$(wildcard $(dir)/*.cpp))
# Déduit tous les fichiers objets .o à partir du nom des fichiers sources
objets := $(sources:%.cpp=$(BUILD_DIR)/%.o)
objetsTest := $(tests:%.cpp=$(BUILD_DIR)/%.o)
# Déduit tous les fichiers dépendances .o à partir des fichiers objets
dependances := $(objets:.o=.d) $(objetsTest:.o=.d)

all: $(TARGET_EXE)

# Créé l'exécutable final avec tous les fichiers objets
$(TARGET_EXE): $(objets)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)
$(TARGET_EXE)-test: $(filter-out $(BUILD_DIR)/src/main.o, $(objets)) $(objetsTest)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS) -lgcov

# Définit la compilation des fichiers C++
# Le dossier de build est créé s'il n'existe pas encore
$(BUILD_DIR)/src/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/test/%.o: test/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -Isrc -c $< -o $@

# Créé le répertoire de build
$(BUILD_DIR):
	mkdir -p $(addprefix $(BUILD_DIR)/,$(SRC_DIR)) $(addprefix $(BUILD_DIR)/,$(TEST_DIR))

# Génère la documentation avec Doxygen
doc/html/index.html: Doxyfile $(foreach dir,$(SRC_DIR),$(wildcard $(dir)/*.h))
	doxygen

# Supprime tous les fichiers issus de la compilation
clean:
	rm -rf build $(TARGET_EXE) doc

doc: doc/html/index.html

# Formate les sources
format:
	find src/ test/ \( -name '*.cpp' -o -name '*.h' \) -exec clang-format -i {} \;

# Couverture des tests
coverage.html: $(TARGET_EXE)-test build/test/test/main.o
	./$(TARGET_EXE)-test && gcovr -r src -d build --html coverage.html

.PHONY: all clean doc format

# Inclut les Makefiles de chaque fichier source
# Le - permet d'ignorer les Makefiles manquants
-include $(dependances)
