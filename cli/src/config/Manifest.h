#pragma once

#include <filesystem>
#include <string>
#include <toml.hpp>
#include <unordered_map>

namespace oki {
    /**
     * Métadonnées du paquet courant.
     */
    class Manifest {
    private:
        toml::v3::table table{};
        toml::v3::table &addDependencySectionIfNotExists();

    public:
        /**
         * Créé un manifeste à partir d'un fichier existant potentiellement.
         */
        static Manifest fromFile(std::filesystem::path fileName);

        /**
         * Liste les dépendances directes indiquées dans ce manifeste.
         */
        std::unordered_map<std::string_view, std::string> listDeclaredPackages() const;

        /**
         * Ajoute un paquet à la liste des dépendances directes. S'il existe déjà, alors il n'est pas rajouté.
         */
        void addDeclaredPackage(std::string_view packageName, std::string_view version);

        /**
         * Créé un manifeste à partir d'un fichier existant potentiellement.
         */
        void loadFileIfExists(std::filesystem::path fileName);

        /**
         * Écrit dans un fichier le manifeste.
         */
        void saveIntoFile(std::filesystem::path fileName);

        friend std::ostream &operator<<(std::ostream &, const Manifest &);
    };
    std::ostream &operator<<(std::ostream &os, const Manifest &manifest);
}
