#pragma once

#include <filesystem>

namespace oki {
    /**
     * Un extracteur d'archives compressées au format ZIP.
     */
    class Extractor {
    private:
        std::filesystem::path destination;
    public:
        /**
         * Créé un nouvel extracteur.
         *
         * @param destination Chemin vers le dossier où extraire les fichiers.
         */
        explicit Extractor(std::filesystem::path destination);

        /**
         * Extraire tous les fichiers à partir de l'archive dont le chemin est spécifié.
         *
         * @param archivePath Le chemin vers l'archive à décompresser.
         */
        void extract(const std::filesystem::path &archivePath);
    };
}
