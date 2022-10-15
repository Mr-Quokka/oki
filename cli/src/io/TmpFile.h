#pragma once

namespace oki{
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
        const char *getFilename();

        /**
         * Supprime le fichier temporaire.
         */
        ~TmpFile();
    };
}
