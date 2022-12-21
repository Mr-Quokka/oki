#pragma once

namespace io {
    /**
     * Un fichier temporaire utilisant le principe de RAII.
     */
    class TmpFile {
    private:
        char filename[20];
        int fd;

    public:
        /**
         * Créé un nouveau fichier temporaire.
         */
        TmpFile();

        /**
         * Récupère le chemin absolu vers le fichier temporaire.
         * @return
         */
        const char *getFilename() const;

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
        int fd;

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
        const char *getDirname() const;

        /**
         * Supprime le dossier temporaire.
         */
        ~TmpDir();
    };
}
