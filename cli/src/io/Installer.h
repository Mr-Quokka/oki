#pragma once

#include "../config/InstallationRegistry.h"
#include "../config/ManifestLock.h"
#include "../make/BuildConfigurer.h"
#include <algorithm>

namespace io {
    /**
     * Représente le résultat de l'installation d'un paquet.
     */
    enum class InstallationResult : unsigned char {
        /**
         * Une nouvelle version a été installée.
         */
        Installed,

        /**
         * Un changement de version a été fait.
         */
        Updated,

        /**
         * Aucune opération n'a été nécessaire.
         */
        NoChange
    };

    /**
     * Un outil pour installer des versions de paquets.
     *
     * Les versions déjà installées sont référencées dans le registre d'installation pour ne pas réinstaller ce qui ets déjà présent.
     *
     * La synchronisation de la liste des paquets installés n'est pas automatique, appeler Installer#saveRegistry après des
     * installations.
     */
    class Installer {
    public:
        /**
         * Un itérateur vers une version actuellement installée d'un paquet.
         */
        using Current = config::InstallationRegistry::Installed::const_iterator;

    private:
        config::InstallationRegistry registry;
        std::filesystem::path workingDirectory;
        std::vector<std::string> pendingConfigurations;

        /**
         * Détermine le chemin où installer une dépendance.
         * @param dependencyName Le nom de la dépendance.
         * @return Le chemin où installer.
         */
        std::filesystem::path getDependencyPath(std::string_view dependencyName);

    public:
        /**
         * Crée un nouvel installateur qui opère dans un répertoire de travail.
         *
         * @param workingDirectory Le répertoire de travail où installer les versions.
         */
        Installer(config::InstallationRegistry &&registry, const std::filesystem::path &workingDirectory);

        /**
         * Installe la version d'un paquet donné.
         *
         * Lorsque le paquet n'est pas déjà installé, il est installé et InstallationResult::Installed est retourné.
         *
         * Un paquet déjà installé sera mis à jour vers la version demandée. InstallationResult::Updated est retourné
         * et la référence de version passée en paramètre est mise à jour avec l'ancienne version.
         *
         * Lorsque exactement cette version est déjà installée, aucun changement n'a été opéré et
         * InstallationResult::NoChange est retourné.
         *
         * @param packageName Le nom du paquet qui possède cette version.
         * @param version La version de travail.
         * @return Le résultat de l'installation.
         */
        std::pair<Current, InstallationResult> install(const std::string &packageName, package::DownloadableVersion &version);

        /**
         * Réalise la configuration des paquets installés.
         *
         * Elle doit être faite après chaque groupe d'installation.
         * @param configurer Le configurateur de construction.
         */
        void configure(make::BuildConfigurer &configurer);

        /**
         * Désinstalle n'importe quelle version d'un paquet donné.
         *
         * @param packageName Le nom du paquet à supprimer.
         * @return true si une désinstallation a eu lieu, false sinon.
         */
        bool uninstall(const std::string &packageName);

        /**
         * Désinstalle tous les paquets qui ne sont pas accessibles.
         *
         * @param isReachable Détermine si un paquet est référencé quelque part.
         * @return Le nombre de paquets désinstallés.
         */
        template <typename F>
        requires std::invocable<F, const std::string &, const package::DownloadableVersion &> &&
            std::convertible_to<std::invoke_result_t<F, const std::string &, const package::DownloadableVersion &>,
                                bool> unsigned int
            uninstallUnreachable(F &&isReachable) {
            unsigned int removed = 0;
            auto it = registry.begin();
            while (it != registry.end()) {
                if (!isReachable(it->first, it->second)) {
                    std::filesystem::remove_all(getDependencyPath(it->first));
                    it = registry.erase(it);
                    ++removed;
                } else {
                    ++it;
                }
            }
            return removed;
        }

        template <std::forward_iterator Iter, std::sentinel_for<Iter> Stop>
        std::unordered_map<InstallationResult, unsigned int> installAll(Iter begin, Stop end) {
            std::unordered_map<InstallationResult, unsigned int> updated;
            std::for_each(begin, end, [&](const auto &pair) {
                updated[install(pair.first, pair.second)] += 1;
            });
            return updated;
        }

        void saveRegistry(const std::filesystem::path &path);
    };
}
