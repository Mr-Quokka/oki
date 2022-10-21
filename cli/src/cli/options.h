#pragma once

#include <string_view>
#include <variant>

namespace oki {
    /**
     * Liste tous les paquets disponibles.
     */
    struct ListAction {
    };

    /**
     * Demande l'installation d'un paquet nommé.
     */
    struct InstallAction {
        std::string_view packageName;
        explicit InstallAction(const char *packageName) : packageName{packageName} {}
    };

    /**
     * Reinstalle tous les paquets present dans le manifeste.
     */
    struct ReInstallAction {
    };

    /**
     * Affiche les informations d'un paquet d'après son nom.
     */
    struct ShowAction {
        std::string_view packageName;
        explicit ShowAction(const char *packageName) : packageName{packageName} {}
    };

    /**
     * Toutes les actions possibles.
     */
    using CliAction = std::variant<ListAction, InstallAction, ShowAction, ReInstallAction>;

    CliAction parseArguments(int argc, char *argv[]);
}
