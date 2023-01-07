#pragma once

#include "../repository/Repository.h"
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
     * @return Le chemin à utiliser.
     */
    std::string_view getDefaultRemoteRepository();

    /**
     * Détermine le chemin vers le fichier de configuration.
     *
     * @return Le chemin à utiliser.
     */
    std::filesystem::path getConfigPath();

    /**
     * Lis un chemin vers un dépôt local ou distant
     *
     * @param source
     * @return Le dépôt.
     */
    std::unique_ptr<repository::Repository> readRepositoryPath(std::string_view source);

    /**
     * Détermine si STDOUT supporte la couleur.
     *
     * @return Si STDOUT est un terminal.
     */
    bool acceptColor();
}
