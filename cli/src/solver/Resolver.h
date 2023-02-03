#pragma once

#include <unordered_map>

#include "../package/VersionRequirement.h"
#include "../repository/GlobalRepository.h"
#include "../repository/Repository.h"
#include "../semver/Range.h"

namespace solver {
    /**
     * Contraint la version d'un paquet.
     */
    using Requirement = package::VersionRequirement;

    /**
     * Les dépendances directes, c'est-à-dire celles qui sont déclarées dans le paquet source.
     */
    using Summaries = std::unordered_map<std::string, Requirement>;

    /**
     * Une solution du graphe de dépendances déterminant les versions transitives à installer.
     */
    using Resolved = std::unordered_map<std::string, package::PackageVersion>;

    /**
     * Une dépendance vers un paquet nommé et avec une exigence.
     *
     * Cette structure est une version mise à plat du type package::Summaries
     */
    struct Dependency {
        std::reference_wrapper<const std::string> dep;
        semver::Range req;
        repository::Repository *repository;
        Dependency(const std::string &dep, const Requirement &req);
    };

    /**
     * Résout le graphe des dépendances d'après les dépendances de premier niveau.
     *
     * D'après des références et des contraintes, cette classe permet de déterminer un ensemble de versions
     * concrètes des dépendances transitives à installer pour faire fonctionner le projet.
     *
     * Résoudre un graphe de contraintes est un problème NP-complet. Pour obtenir un résultat en temps
     * raisonnable, l'algorithme va donc s'appuyer sur l'heuristique pour d'abord essayer les versions
     * qui sont le plus susceptible d'être compatibles entre elles.
     *
     * L'algorithme employé ici teste en théorie toutes exhaustivement toutes les combinaisons possibles
     * jusqu'à trouver une qui fonctionne. Pour plus de détails, consulter ARCHITECTURE.md
     */
    class Resolver {
    private:
        using Registry = std::unordered_map<std::string, package::Package>;

        repository::GlobalRepository &repository;
        Registry registry;

        /**
         * Récupère les versions disponibles d'un paquet d'après son nom.
         *
         * Cette méthode est un proxy vers le dépôt utilisé, puisqu'elle garde en mémoire les paquets déjà recherchés.
         *
         * @param dep La dépendance à rechercher.
         * @return Les versions disponibles.
         */
        const std::vector<package::PackageVersion> &getVersions(const Dependency &dep);

    public:
        explicit Resolver(repository::GlobalRepository &repository);
        Resolved resolve(const solver::Summaries &summaries);
        Resolved resolve(const std::vector<Dependency> &dependencies);
    };

    Resolved resolve(const package::Summaries &summaries, repository::GlobalRepository &repository);
    Resolved resolve(const solver::Summaries &summaries, repository::GlobalRepository &repository);
    Resolved resolve(const std::vector<Dependency> &dependencies, repository::GlobalRepository &repository);
}
