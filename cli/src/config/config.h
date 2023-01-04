#pragma once

#include <filesystem>

namespace config {
    /**
     * Détermine le chemin par défaut vers le répertoire local de l'utilisateur.
     *
     * @return Le chemin à utiliser.
     */
    std::filesystem::path getDefaultLocalRepository();

    /**
     * Détermine le chemin par défaut vers le répertoire distant.
     * 
     * @return Le chemin à utiliser
     */
    std::string_view getDefaultRemoteRepository();

    /**
     * Détermine si STDOUT supporte la couleur.
     *
     * @return Si STDOUT est un terminal.
     */
    bool acceptColor();
}
