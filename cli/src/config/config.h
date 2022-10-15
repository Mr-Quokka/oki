#pragma once

#include <filesystem>

namespace oki{
    /**
     * Détermine le chemin par défaut vers le répertoire local de l'utilisateur.
     *
     * @return Le chemin à utiliser.
     */
	std::filesystem::path getDefaultLocalRepository();

    /**
     * Détermine si STDOUT supporte la couleur.
     *
     * @return Si STDOUT est un terminal.
     */
	bool acceptColor();
}
