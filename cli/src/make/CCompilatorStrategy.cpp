#include "CCompilatorStrategy.h"

namespace make {
    std::ostream &CCompilatorStrategy::writeStart(std::ostream &out) {
        return out << "CC := gcc\nCFLAGS := -std=c17 -Wall -Wextra -pedantic -g -MMD -MP\n";
    }

    std::ostream &CCompilatorStrategy::writeEnd(std::ostream &out) {
        return out << "\nsources := $(wildcard $(SRC_DIR)/*.c)\n"
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
    }

    std::ostream &CCompilatorStrategy::writeMain(std::ostream &out) {
        return out << "#include <stdio.h>\n\n"
                    << "int main(void) {\n"
                    << "    printf(\"Hello world!\\n\");\n"
                    << "    return 0;\n"
                    << "}\n";
    }

        std::string_view CCompilatorStrategy::getMainName(){
        return "main.c";
    }
}
