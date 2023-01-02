#pragma once

#include "../make/ProjectKind.h"
#include "CliAction.h"

namespace cli {
    class InitAction : public CliAction {
    private:
        std::string projectName;
        make::ProjectKind projectKind;

    public:
        explicit InitAction(make::ProjectKind kind);
        InitAction(const char *projectName, const make::ProjectKind kind);
        void run(repository::Repository &repository) override;
    };
}
