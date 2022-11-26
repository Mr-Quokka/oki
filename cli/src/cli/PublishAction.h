#pragma once

#include "CliAction.h"

namespace cli {
    class PublishAction : public CliAction {
        void run(repository::Repository &repository) override;
    };
}
