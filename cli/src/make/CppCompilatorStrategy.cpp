#include "CppCompilatorStrategy.h"

namespace make {
    std::ostream &CppCompilatorStrategy::writeMakefileStart(std::ostream &out) {
        return out << "CXX := g++\nCXXFLAGS := -std=c++20 -Wall -Wextra -pedantic -g -MMD -MP\n";
    }

    std::ostream &CppCompilatorStrategy::writeMakefileEnd(std::ostream &out) {
        return out << "\nsources := $(wildcard $(SRC_DIR)/*.cpp)\n"
                      "objets := $(sources:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)\n"
                      "dependances := $(objets:.o=.d)\n"
                      "\n$(TARGET_EXE): $(objets) | oki-built-dependencies\n"
                      "\t$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)\n"
                      "\n$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)\n"
                      "\t$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@\n"
                      "\n$(BUILD_DIR):\n"
                      "\tmkdir $(BUILD_DIR)\n"
                      "\n.PHONY: clean oki-built-dependencies\n"
                      "clean:\n"
                      "\trm -rf $(BUILD_DIR) $(TARGET_EXE)\n"
                      "oki-built-dependencies:\n"
                      "\toki build\n"
                      "\n-include $(dependances)\n";
    }

    std::ostream &CppCompilatorStrategy::writeSampleMain(std::ostream &out) {
        return out << "#include <iostream>\n\n"
                   << "int main() {\n"
                   << "    std::cout << \"Hello world!\\n\";\n"
                   << "    return 0;\n"
                   << "}\n";
    }

    std::string_view CppCompilatorStrategy::getMainFileName() {
        return "main.cpp";
    }
}
