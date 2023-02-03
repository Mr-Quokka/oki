#pragma once

#include "../semver/Range.h"
#include "../semver/Version.h"
#include <optional>
#include <string>

namespace package {
    /**
     * Une dépendance à une version d'un paquet.
     */
    class VersionRequirement {
    private:
        semver::Range range;
        std::optional<std::string> registry;

    public:
        /**
         * Crée une nouvelle dépendance à une version d'un paquet.
         *
         * @param range L'intervalle de versions acceptées.
         * @param registry Le nom du registre à utiliser pour cette dépendance.
         */
        VersionRequirement(const semver::Range &range, const std::optional<std::string> &registry);

        /**
         * Crée une nouvelle dépendance à une version d'un paquet.
         *
         * @param range L'intervalle de versions acceptées.
         * @param registry Le nom du registre à utiliser pour cette dépendance.
         */
        VersionRequirement(semver::Range &&version, std::optional<std::string> &&registry);

        /**
         * Récupère l'intervalle de versions acceptées.
         *
         * @return L'intervalle de versions acceptées.
         */
        const semver::Range &getRange() const;

        /**
         * Récupère le nom du registre à utiliser.
         *
         * @return Le nom du registre à utiliser.
         */
        const std::optional<std::string> &getRegistry() const;
    };
}
