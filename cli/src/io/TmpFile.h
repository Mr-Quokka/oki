#pragma once

#include <filesystem>

namespace io {
    /**
     * Un fichier temporaire utilisant le principe de RAII.
     */
    class TmpFile {
    private:
        char filename[20];
        int fd;
        std::filesystem::path filePath;

    public:
        /**
         * Créé un nouveau fichier temporaire.
         */
        TmpFile();

        /**
         * Récupère le chemin absolu vers le fichier temporaire.
         *
         * @return Le chemin absolu.
         */
        const std::filesystem::path &path() const;

        /**
         * Supprime le fichier temporaire.
         */
        ~TmpFile();
    };

    /**
     * Un dossier temporaire utilisant le principe de RAII.
     */
    class TmpDir {
    private:
        char dirname[20];
        std::filesystem::path dirPath;

    public:
        /**
         * Créé un nouveau dossier temporaire.
         */
        TmpDir();

        /**
         * Récupère le chemin absolu vers le dossier temporaire.
         *
         * @return Le chemin absolu.
         */
        const std::filesystem::path &path() const;

        /**
         * Supprime le dossier temporaire.
         */
        ~TmpDir();
    };
}
