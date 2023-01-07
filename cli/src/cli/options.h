#pragma once

#include <memory>
#include <span>

#include "../config/UserConfig.h"
#include "CliAction.h"

namespace cli {
    /**
     * Toutes les actions possibles.
     *
     * @param config La configuration de l'utilisateur.
     * @param args Les arguments de la ligne de commande.
     */
    std::unique_ptr<CliAction> parseArguments(config::UserConfig &config, std::span<const char *> args);
}
