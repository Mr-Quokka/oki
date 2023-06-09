#pragma once

#include <filesystem>
#include <span>

namespace io {
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

    /**
     * Un extracteur d'archives compressées au format ZIP.
     */
    class Compressor {
    private:
        std::filesystem::path destination;

    public:
        /**
         * Créé un nouvel extracteur.
         *
         * @param destination Chemin vers le dossier où extraire les fichiers.
         */
        explicit Compressor(std::filesystem::path destination);

        /**
         * Compresse tous les fichiers dont le chemin est spécifié.
         *
         * @param files Les fichiers à compresser.
         * @param relativeTo Le chemin relatif à utiliser pour les fichiers.
         */
        void compress(std::span<const std::filesystem::path> files, const std::filesystem::path &relativeTo);
    };
}
