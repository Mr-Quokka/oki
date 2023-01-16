#pragma once

#include "../repository/GlobalRepository.h"
#include <memory>
#include <string>
#include <vector>

namespace config {
    /**
     * La configuration globale de l'utilisateur.
     */
    class UserConfig {
    private:
        repository::GlobalRepository sources;

    public:
        /**
         * Crée une nouvelle configuration.
         *
         * @param sources Les sources des paquets.
         */
        explicit UserConfig(repository::GlobalRepository &&sources);

        /**
         * Récupère un dépôt par son nom.
         *
         * @param name Le nom du dépôt.
         * @return Le dépôt.
         * @throws std::invalid_argument Si le dépôt n'existe pas.
         */
        repository::Repository &getRepository(std::string_view name);

        /**
         * Récupère un dépôt par son nom.
         *
         * @param name Le nom du dépôt.
         * @return Le dépôt.
         * @throws std::invalid_argument Si le dépôt n'existe pas.
         */
        const repository::Repository &getRepository(std::string_view name) const;

        /**
         * Récupère la liste des dépôts.
         *
         * @return La liste des dépôts.
         */
        repository::GlobalRepository &getGlobalRepository();

        /**
         * Lis le fichier de configuration s'il existe, ou utilise une configuration par défaut.
         *
         * @param path Le chemin vers le fichier de configuration.
         * @return La configuration.
         */
        static UserConfig fromFile(const std::filesystem::path &path);
    };
}
