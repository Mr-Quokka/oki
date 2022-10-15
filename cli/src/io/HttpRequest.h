#pragma once

#include <filesystem>
#include <string>

namespace oki {
    /**
     * Une requête HTTP, préparée par CURL.
     */
    class HttpRequest {
    private:
        void *curl;
        std::string url;
    public:
        /**
         * Instancie une nouvelle requête vers une url.
         *
         * @param url L'url à utiliser.
         */
        explicit HttpRequest(std::string_view url);

        /**
         * Exécute la requête avec une méthode GET et capture le résultat dans une chaîne de caractères.
         *
         * @return Le contenu de la réponse du serveur.
         */
        std::string get();

        /**
         * Exécute la requête avec une méthode GET et télécharge la réponse dans un fichier.
         *
         * @param path Le chemin vers le fichier où télécharger.
         */
        void download(const std::filesystem::path &path);

        /**
         * Vide la requête.
         */
        ~HttpRequest();
    };

    /**
     * Une erreur lors de la transmission de la requête.
     */
    class RequestException : public std::exception {
    private:
        int code;
    public:
        explicit RequestException(int code);
        const char *what() const noexcept override;
    };

    /**
     * Une erreur retournée par l'API.
     */
    class APIException : public std::exception {
    private:
        std::string msg;
    public:
        explicit APIException(std::string_view msg);
        const char *what() const noexcept override;
    };
}
