#pragma once

#include "semver/Version.h"
#include <filesystem>
#include <string>
#include <toml.hpp>
#include <unordered_map>

namespace config {
    /**
     * Métadonnées du paquet courant.
     */
    class Manifest {
    private:
        toml::v3::table table{};
        toml::v3::table &addDependencySectionIfNotExists();

    public:
        /**
         * Crée un manifeste à partir d'un fichier existant potentiellement.
         */
        static Manifest fromFile(const std::filesystem::path &fileName);

        /**
         * Liste les dépendances directes indiquées dans ce manifeste.
         */
        std::unordered_map<std::string_view, std::string> listDeclaredPackages() const;

        /**
         * Ajoute un paquet à la liste des dépendances directes. S'il existe déjà, alors il n'est pas rajouté.
         *
         * @return Vrai si la dépendance n'était pas déjà référencée, faux sinon.
         */
        bool addDeclaredPackage(std::string_view packageName, std::string_view version);

        /**
         * Ajoute un paquet à la liste des dépendances directes. S'il existe déjà, alors il n'est pas rajouté.
         *
         * @return Vrai si la dépendance n'était pas déjà référencée, faux sinon.
         */
        bool addDeclaredPackage(std::string_view packageName, semver::Version &version);

        /**
         * Créé un manifeste à partir d'un fichier existant potentiellement.
         */
        bool loadFileIfExists(const std::filesystem::path &fileName);

        /**
         * Écrit dans un fichier le manifeste.
         */
        void saveFile(const std::filesystem::path &fileName);

        friend std::ostream &operator<<(std::ostream &, const Manifest &);
    };
    std::ostream &operator<<(std::ostream &os, const Manifest &manifest);

    /**
     * Une erreur retournée par le fichier Manifest.
     */
    class ManifestException : public std::exception {
    private:
        std::string msg;

    public:
        explicit ManifestException(std::string_view msg);
        const char *what() const noexcept override;
    };
}
