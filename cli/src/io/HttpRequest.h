#pragma once

#include <filesystem>
#include <string>

namespace io {
    /**
     * Décrit la réponse du serveur après une requête HTTP.
     */
    class HttpResponse {
    private:
        int statusCode;
        std::string contentType;
        std::string content;

    public:
        HttpResponse(int statusCode, std::string contentType, std::string content);

        /**
         * Récupère le code de statut HTTP.
         *
         * @return Le statut de la réponse.
         */
        int getStatusCode() const;

        /**
         * Récupère le type MIME retourné par le serveur.
         *
         * Si aucune en-tête n'a été trouvée, alors le retour est une chaîne de caractère vide.
         *
         * @return Le type MIME.
         */
        const std::string &getContentType() const;

        /**
         * Retourne le contenu de la réponse.
         *
         * @return Le contenu.
         */
        const std::string &getContent() const;
    };

    /**
     * Une requête HTTP, préparée par CURL.
     */
    class HttpRequest {
    private:
        void *curl;
        void *headers;
        std::string url;

    public:
        /**
         * Instancie une nouvelle requête vers une url.
         *
         * @param url L'url à utiliser.
         */
        explicit HttpRequest(std::string_view url);

        /**
         * Copie les paramètres d'une requête HTTP.
         *
         * @param request La requête à copier.
         */
        HttpRequest(const HttpRequest &request);

        /**
         * Copie et assigne les paramètres d'une requête HTTP.
         *
         * @param request La requête à copier.
         */
        HttpRequest &operator=(HttpRequest other);

        /**
         * Ajoute une nouvelle en-tête HTTP, sans vérifier les doublons.
         *
         * @param header Le contenu de l'en-tête.
         */
        void addHeader(const std::string &header);

        /**
         * Ajoute une nouvelle en-tête HTTP, sans vérifier les doublons.
         *
         * @param header Le contenu de l'en-tête (terminé par le caractère null '\0').
         */
        void addHeader(const char *header);

        /**
         * Exécute la requête avec une méthode GET et capture le résultat dans une chaîne de caractères.
         *
         * @return Le contenu de la réponse du serveur.
         */
        HttpResponse get();

        /**
         * Exécute la requête avec une méthode GET et télécharge la réponse dans un fichier.
         *
         * @param path Le chemin vers le fichier où télécharger.
         */
        void download(const std::filesystem::path &path);

        /**
         * Récupère l'url de la requête.
         *
         * @return L'url complète.
         */
        const std::string &getUrl() const;

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
