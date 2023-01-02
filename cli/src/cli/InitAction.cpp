#include "InitAction.h"
#include "../io/oki.h"
#include "../op/init.h"
#include <iostream>

namespace fs = std::filesystem;

namespace cli {
    InitAction::InitAction(make::ProjectKind kind)
        : projectName{fs::current_path().filename()}, projectKind{kind} {}

    InitAction::InitAction(const char *projectName, make::ProjectKind kind)
        : projectName{projectName}, projectKind{kind} {}

    void InitAction::run([[maybe_unused]] repository::Repository &repository) {
        if (fs::exists(OKI_MANIFEST_FILE)) {
            std::cerr << "`oki init` cannot be run on already existing packages\n";
            exit(1);
        }
        op::init(op::InitOptions{projectName, projectKind, false}, fs::current_path());
    }
}
