#pragma once

#include <algorithm>

#include "Version.h"

namespace semver {
    /**
     * Un intervalle entre deux versions.
     */
    class Range {
    private:
        Version minInclusive;
        Version maxExclusive;

    public:
        /**
         * Crée un nouvel intervalle ne contenant qu'une version.
         *
         * @param exact La version exacte contenue.
         */
        explicit Range(const Version &exact);

        /**
         * Crée un nouvel intervalle à partir d'une version minimale et d'une maximale.
         *
         * @param minInclusive La borne inférieure de l'intervalle (inclusive).
         * @param maxExclusive La borne supérieure de l'intervalle (exclusive).
         */
        Range(const Version &minInclusive, const Version &maxExclusive);

        /**
         * Détermine si une version donnée est comprise dans cet intervalle.
         *
         * @param version La version à tester.
         * @return true si la version est dans l'intervalle, false sinon.
         */
        bool contains(const Version &version) const;

        /**
         * Récupère l'intersection de deux intervalles de version.
         *
         * L'intervalle retourné contient toutes les versions présentes dans les deux intervalles.
         *
         * @param other L'autre intervalle.
         * @return L'intersection des deux.
         */
        Range intersection(const Range &other) const;

        /**
         * Teste si l'intervalle ne peut contenir aucune version.
         *
         * @return true si l'intervalle est vide, false sinon.
         */
        bool isEmpty() const;

        /**
         * Teste si deux intervalles sont identiques.
         *
         * @param other L'autre intervalle avec lequel comparer.
         * @return true si les intervalles sont identiques, false sinon.
         */
        bool operator==(const Range &other) const;

        /**
         * Teste si deux intervalles sont différentes.
         *
         * @param other L'autre intervalle avec lequel comparer.
         * @return true si les intervalles sont différentes, false sinon.
         */
        bool operator!=(const Range &other) const;

        /**
         * Cherche la première version comprise dans cet intervalle.
         *
         * @tparam Iter L'itérateur à utiliser.
         * @tparam Stop L'itérateur à utiliser pour marquer la fin.
         * @param begin Le début de l'itérateur.
         * @param end La fin de l'itérateur.
         * @return Un itérateur sur la version, ou end si rien ne correspond.
         */
        template <std::forward_iterator Iter, std::sentinel_for<Iter> Stop>
        Iter findSatisfying(Iter begin, const Stop end) const {
            return std::find_if(begin, end, [&](const Version &version) { return contains(version); });
        }

        /**
         * Crée une nouvelle intervalle en interprétant une chaîne de caractères.
         *
         * Lorsque la chaîne de caractères n'est pas dans un format valide, une ParseException est levée.
         *
         * @param s La chaîne de caractères à lire.
         * @return L'intervalle lu.
         * @throw semver::ParseException Si l'intervalle ne peut pas être lu.
         */
        static Range parse(std::string_view s);
    };
}
