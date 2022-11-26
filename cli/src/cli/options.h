#pragma once

#include <string_view>
#include <variant>

#include "CliAction.h"

namespace cli {
    /**
     * Toutes les actions possibles.
     */
    std::unique_ptr<CliAction> parseArguments(int argc, char *argv[]);
}
