#pragma once

#include "../package/DownloadableVersion.h"
#include <filesystem>
#include <unordered_map>

namespace config {
    /**
     * Le registre des installations en vigueur.
     *
     * Il maintient à jour l'information de quelle version de quel paquet est actuellement installée.
     */
    class InstallationRegistry {
    public:
        using Installed = std::unordered_map<std::string, package::DownloadableVersion>;

    private:
        Installed alreadyInstalled;

    public:
        /**
         * Obtiens un itérateur au début du registre.
         *
         * @return Le début du registre.
         */
        Installed::const_iterator cbegin() const;

        /**
         * Obtiens un itérateur à la fin du registre.
         *
         * @return La fin du registre.
         */
        Installed::const_iterator cend() const;

        /**
         * Recherche si un paquet est installé.
         *
         * @param packageName Le nom du paquet à rechercher.
         * @return Un itérateur sur les informations ou cend.
         */
        Installed::const_iterator find(const std::string &packageName) const;

        /**
         * Vérifie si un paquet est installé dans une version donnée.
         *
         * @param packageName Le nom du paquet à vérifier.
         * @param version La version à vérifier.
         * @return true si le paquet est installé dans exactement la version demandée, false sinon.
         */
        bool containsExact(const std::string &packageName, const package::DownloadableVersion &version) const;

        /**
         * Insère sur place l'information d'un paquet installé.
         *
         * Si le paquet est déjà installé (même dans une autre version), alors rien n'est changé.
         *
         * @param packageName Le nom du paquet à référencer.
         * @param version La version à insérer.
         * @return Un itérateur vers la valeur insérée ou celle qui existait déjà, et un booléen si l'insertion a eu
         * lieu.
         */
        std::pair<Installed::iterator, bool> emplace(const std::string &packageName, const package::DownloadableVersion &version);

        /**
         * Supprime la référence à un paquet installé.
         *
         * @param packageName Le nom du paquet à marquer comme supprimé.
         * @return true si le paquet était référencé et qu'il ne l'est plus, false sinon.
         */
        bool erase(const std::string &packageName);

        /**
         * Charge un registre depuis un fichier s'il existe.
         *
         * S'il n'existe pas, il est créé vide.
         *
         * @param path Le chemin vers le fichier des données au format TOML.
         * @return Le registre, complété avec les données du fichier s'il existe.
         */
        static InstallationRegistry loadFileIfExists(const std::filesystem::path &path);
        void saveToFile(const std::filesystem::path &path);
    };
}
