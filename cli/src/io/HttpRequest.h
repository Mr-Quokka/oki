#pragma once

#include <filesystem>
#include <string>

namespace io {
    enum class HttpAuth : unsigned char {
        Basic,
        Digest
    };

    class HttpCode {
    private:
        long code;

    public:
        explicit HttpCode(long code);
        long getCode() const;
        bool isError() const;
    };

    /**
     * Décrit la réponse du serveur après une requête HTTP.
     */
    class HttpResponse {
    private:
        HttpCode statusCode;
        std::string contentType;
        std::string content;
        long auth;

    public:
        HttpResponse(HttpCode statusCode, std::string contentType, std::string content, long auth);

        /**
         * Récupère le code de statut HTTP.
         *
         * @return Le statut de la réponse.
         */
        HttpCode getStatusCode() const;

        bool requiresAuth() const;

        bool requiresBasicAuth() const;

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
     * Un contenu de requête HTTP, au type de contenu multipart/form-data.
     *
     * Cela permet notamment d'envoyer des données de formulaire ou des fichiers dans des requêtes POST.
     */
    class MimePart {
    private:
        void *form;

    public:
        explicit MimePart(void *form);
        MimePart(const MimePart &part) = delete;

        /**
         * Ajoute une nouvelle donnée depuis des données en mémoire.
         *
         * @param name La clé identifiant les données.
         * @param data Les données à transmettre.
         */
        void addDataPart(const std::string &name, std::string_view data);

        /**
         * Ajoute une nouvelle donnée depuis des données en mémoire.
         *
         * @param name La clé identifiant les données.
         * @param data Les données à transmettre.
         */
        void addDataPart(const char *name, std::string_view data);

        /**
         * Ajoute une nouvelle donnée à partir du contenu d'un fichier.
         *
         * @param name La clé identifiant les données.
         * @param data Le chemin vers le fichier pour lequel inclure le contenu.
         */
        void addFilePart(const std::string &name, const std::filesystem::path &data);

        /**
         * Ajoute une nouvelle donnée à partir du contenu d'un fichier.
         *
         * @param name La clé identifiant les données.
         * @param data Le chemin vers le fichier pour lequel inclure le contenu.
         */
        void addFilePart(const char *name, const std::filesystem::path &data);

        ~MimePart();
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

        static io::HttpRequest createJson(std::string_view url);

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

        MimePart addMime();

        /**
         * Ajoute des informations de connexion HTTP à la requête.
         *
         * @param auth Le support d'authentification.
         * @param username Le nom d'utilisateur.
         * @param password Le mot de passe.
         */
        void authenticate(HttpAuth auth, const std::string &username, const std::string &password);

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
        HttpCode download(const std::filesystem::path &path);

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
    };

    class CurlException : public RequestException {
    private:
        int code;

    public:
        explicit CurlException(int code);
        const char *what() const noexcept override;
    };
}
