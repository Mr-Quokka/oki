#pragma once

#include "CliAction.h"
#include "Command.h"

namespace cli {
    class PublishAction : public CliAction {
    private:
        repository::Repository &registry;

    public:
        PublishAction(config::UserConfig &config, ArgMatches &&args);

        void run() override;

        static Command cmd();
    };
}
