#include "Resolver.h"

#include <algorithm>
#include <functional>
#include <unordered_set>

namespace solver {
    Dependency::Dependency(const std::string &dep, const semver::Range &range)
        : dep{dep}, range{range} {}

    Resolver::Resolver(repository::Repository &repository)
        : repository{repository} {}

    const std::vector<package::PackageVersion> &Resolver::getVersions(const std::string &packageName) {
        auto existing = registry.find(packageName);
        if (existing == registry.end()) {
            package::Package package = repository.getPackageInfo(packageName);
            existing = registry.insert(std::make_pair(packageName, std::move(package))).first;
        }
        return existing->second.getVersions();
    }

    Resolved Resolver::resolve(const Summaries &summaries) {
        std::vector<Dependency> dependencies;
        std::transform(
            summaries.cbegin(),
            summaries.end(),
            std::back_inserter(dependencies),
            [](const auto &pair) {
                return Dependency{pair.first, pair.second};
            });
        return resolve(dependencies);
    }

    Resolved Resolver::resolve(const std::vector<Dependency> &dependencies) {
        using Solution = std::unordered_map<std::string, const package::PackageVersion *>;

        // Garder une trace d'une future solution
        Solution solution;

        const std::string *lastSeen = nullptr;

        // Fonction récursive pour trouver la version la plus récente et compatible pour un package donné
        std::function<bool(const Solution &, const std::vector<Dependency> &)> backtrack =
            [&](const Solution &partialSolution, const std::vector<Dependency> &remainingDependencies) -> bool {
            // S'il n'y a plus de dépendances restantes, la solution partielle est complète et nous pouvons la retourner comme solution finale
            if (remainingDependencies.empty()) {
                solution = partialSolution;
                return true;
            }

            // Prendre la première dépendance restante
            const Dependency &dep = remainingDependencies.front();
            lastSeen = &dep.dep.get();

            // Vérifier que la dépendance n'a pas déjà été sélectionnée avec une version incompatible
            auto alreadyActivated = partialSolution.find(dep.dep);
            if (alreadyActivated != partialSolution.cend() && !dep.range.contains(*alreadyActivated->second)) {
                return false;
            }

            // Obtenir les versions disponibles pour ce package à partir du registre
            const auto &versions = getVersions(dep.dep);

            // Parcourir les versions, les plus récentes en premier
            for (const package::PackageVersion &version : versions) {
                // Sauter cette version si elle n'est pas dans l'intervalle
                if (!dep.range.contains(version)) {
                    continue;
                }

                // Créer une nouvelle solution partielle en ajoutant la version actuelle
                Solution newPartialSolution = partialSolution;
                newPartialSolution.emplace(dep.dep, &version);

                // Ajouter les dépendances de la version actuelle à la liste des dépendances restantes
                std::vector<Dependency> newRemainingDependencies{remainingDependencies.cbegin() + 1, remainingDependencies.cend()};
                std::transform(
                    version.getDependencies().cbegin(),
                    version.getDependencies().cend(),
                    std::back_inserter(newRemainingDependencies),
                    [&](const auto &pair) -> Dependency {
                        return {pair.first, pair.second};
                    });

                // Récursion sur la nouvelle solution partielle et la nouvelle liste de dépendances restantes
                if (backtrack(newPartialSolution, newRemainingDependencies)) {
                    return true;
                }
            }
            return false;
        };

        if (backtrack({}, dependencies)) {
            Resolved resolved;
            std::transform(
                solution.cbegin(),
                solution.cend(),
                std::inserter(resolved, resolved.end()),
                [](const auto &pair) {
                    return std::make_pair(pair.first, *pair.second);
                });
            return resolved;
        } else {
            throw std::range_error{"no matching version found for package " + *lastSeen};
        }
    }

    Resolved resolve(const solver::Summaries &summaries, repository::Repository &repository) {
        return Resolver{repository}.resolve(summaries);
    }

    Resolved resolve(const std::vector<Dependency> &dependencies, repository::Repository &repository) {
        return Resolver{repository}.resolve(dependencies);
    }
}
