#pragma once

#include <filesystem>
#include <string>
#include <toml.hpp>
#include <unordered_map>

#include "io/Checksum.h"
#include "make/BuildInfo.h"
#include "make/ProjectKind.h"
#include "package/Summary.h"
#include "semver/Range.h"

namespace config {
    /**
     * Métadonnées du paquet courant.
     */
    class Manifest {
    private:
        toml::table table{};
        toml::table &addDependencySectionIfNotExists();
        const toml::table &getPackageSection() const;

    public:
        /**
         * Crée un manifeste à partir d'un fichier existant potentiellement.
         */
        static Manifest fromFile(const std::filesystem::path &fileName);

        /**
         * Liste les dépendances directes indiquées dans ce manifeste.
         */
        package::Summaries listDeclaredPackages() const;

        /**
         * Liste les noms des dépendances directes indiquées dans ce manifeste.
         */
        std::vector<std::string> listDeclaredPackagesNames() const;

        /**
         * Récupère le nom de projet.
         *
         * @return Le nom du projet.
         */
        std::string_view getProjectName() const;

        /**
         * Récupère le chemin des sources.
         */
        std::vector<std::filesystem::path> getInclude() const;

        /**
         * Récupère le type de projet.
         *
         * @return Le type du projet.
         */
        make::ProjectKind getProjectKind() const;

        /**
         * Récupère les informations de construction.
         *
         * @return Les informations de construction.
         */
        make::BuildInfo getBuildInfo() const;

        /**
         * Récupére la version du paquet dans le fichier manifeste.
         *
         * @return La version du paquet.
         */
        semver::Version getPackageVersion() const;

        /**
         * Ajoute un paquet à la liste des dépendances directes. S'il existe déjà, alors il n'est pas rajouté.
         *
         * @return Vrai si la dépendance n'était pas déjà référencée, faux sinon.
         */
        bool addDeclaredPackage(std::string_view packageName, std::string_view version);

        /**
         * Retire un paquet de la liste des dépendances directes.
         *
         * @return Vrai si la dépendance était déjà référencée, faux sinon.
         */
        bool removeDeclaredPackage(std::string_view packageName);

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

        /**
         * Sérialise les données du manifeste au format JSON.
         *
         * @return Les données publiques de version au format JSON.
         */
        std::string asFilteredJson(const io::Checksum &checksum) const;

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
