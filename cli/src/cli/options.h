#pragma once

#include <string_view>
#include <variant>

#include "CliAction.h"

namespace oki {
    /**
     * Toutes les actions possibles.
     */
    CliAction *parseArguments(int argc, char *argv[]);
}
