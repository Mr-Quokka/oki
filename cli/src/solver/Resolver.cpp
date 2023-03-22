#include "Resolver.h"

#include <algorithm>
#include <functional>
#include <unordered_set>

namespace solver {
    Dependency::Dependency(const std::string &dep, const Requirement &req)
        : dep{dep}, req{req.getRange()}, repository{nullptr} {}

    Resolver::Resolver(repository::GlobalRepository &repository)
        : repository{repository} {}

    const std::vector<package::PackageVersion> &Resolver::getVersions(const Dependency &dep) {
        auto existing = registry.find(dep.dep);
        if (existing == registry.end()) {
            auto [package, selectedRepository] = repository.getPackageInfo(dep.dep.get(), dep.repository);
            existing = registry.insert(std::make_pair(dep.dep, std::move(package))).first;
        }
        return existing->second.getVersions();
    }

    Resolved Resolver::resolve(const Summaries &summaries) {
        std::vector<Dependency> dependencies;
        std::transform(
            summaries.cbegin(),
            summaries.end(),
            std::back_inserter(dependencies),
            [this](const auto &pair) {
                Dependency dep{pair.first, pair.second};
                if (pair.second.getRegistry().has_value()) {
                    dep.repository = &repository.getRepository(pair.second.getRegistry().value());
                }
                return dep;
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
            if (alreadyActivated != partialSolution.cend() && !dep.req.contains(*alreadyActivated->second)) {
                return false;
            }

            // Obtenir les versions disponibles pour ce package à partir du registre
            const auto &versions = getVersions(dep);

            // Parcourir les versions, les plus récentes en premier
            for (const package::PackageVersion &version : versions) {
                // Sauter cette version si elle n'est pas dans l'intervalle
                if (!dep.req.contains(version)) {
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
                        return {pair.first, {pair.second, dep.dep}};
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

    Resolved resolve(const package::Summaries &summaries, repository::GlobalRepository &repository) {
        solver::Summaries solverSummaries;
        std::transform(
            summaries.cbegin(),
            summaries.cend(),
            std::inserter(solverSummaries, solverSummaries.end()),
            [](const auto &pair) {
                return std::make_pair(pair.first, Requirement{pair.second, std::nullopt});
            });
        return Resolver{repository}.resolve(solverSummaries);
    }

    Resolved resolve(const solver::Summaries &summaries, repository::GlobalRepository &repository) {
        return Resolver{repository}.resolve(summaries);
    }

    Resolved resolve(const std::vector<Dependency> &dependencies, repository::GlobalRepository &repository) {
        return Resolver{repository}.resolve(dependencies);
    }
}
