#pragma once

#include <compare>
#include <limits>
#include <ostream>
#include <string_view>

namespace semver {
    /**
     * Une version raccourcie de https://semver.org/lang/fr
     */
    struct Version {
        using Value = unsigned int;

        static constexpr Value MaxValue = std::numeric_limits<Value>::max();

        /**
         * Le numéro de version MAJEUR quand il y a des changements non rétrocompatibles.
         */
        Value major;

        /**
         * Le numéro de version MINEUR quand il y a des ajouts de fonctionnalités rétrocompatibles.
         */
        Value minor;

        /**
         * Le numéro de version de CORRECTIF quand il y a des corrections d’anomalies rétrocompatibles.
         */
        Value patch;

        /**
         * Crée une version vide, c'est-à-dire 0.0.0.
         */
        Version() = default;

        /**
         * Crée une version à partir de ses composants.
         */
        Version(Value major, Value minor, Value patch);

        /**
         * Récupère la version minimale juste après la version courante.
         *
         * @return La version minimale suivante.
         * @throw std::range_error Si la version est maximale (égale à #maxVersion).
         */
        Version nextAfter() const;

        /**
         * Récupère une représentation textuelle de la version.
         *
         * @return Une chaîne de caractères.
         */
        std::string str() const;

        /**
         * Compare deux versions.
         *
         * @return Le résultat de la comparaison.
         */
        std::strong_ordering operator<=>(const Version &) const = default;

        /**
         * Formate la version dans un flux.
         *
         * @param out Le flux.
         * @return Le même flux.
         */
        friend std::ostream &operator<<(std::ostream &out, const Version &);

        /**
         * Récupère une représentation textuelle de la version.
         *
         * @return Une chaîne de caractères.
         */
        operator std::string() const;

        /**
         * Crée une nouvelle version en interprétant une chaîne de caractères.
         *
         * @param s La chaîne de caractères à lire.
         * @return La version lue.
         * @throw semver::ParseException Si la version ne peut pas être lue.
         */
        static Version parse(std::string_view s);

        /**
         * Crée une nouvelle version à partir d'une chaîne de caractères avec un contexte en cas d'erreur.
         *
         * @param s La chaîne de caractères à lire.
         * @param ctx Le contexte autour de la chaîne de caractères à inclure en cas d'erreur.
         * @return La version lue.
         * @throw semver::ParseException Si la version ne peut pas être lue.
         */
        static Version parseWithContext(std::string_view s, std::string_view ctx);

        /**
         * Retourne la version représentable la plus grande.
         *
         * @return La plus grande version.
         */
        static Version maxVersion() {
            return {MaxValue, MaxValue, MaxValue};
        }
    };

    /**
     * Formate la version dans un flux.
     *
     * @param out Le flux.
     * @return Le même flux.
     */
    std::ostream &operator<<(std::ostream &out, const Version &version);
}
