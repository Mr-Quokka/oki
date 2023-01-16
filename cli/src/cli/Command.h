#pragma once

#include "../config/UserConfig.h"
#include "ArgMatches.h"
#include "CliAction.h"
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/positional_options.hpp>
#include <memory>
#include <span>

namespace cli {
    /**
     * Représente une commande nommée avec des arguments.
     *
     * @example InitAction::cmd()
     */
    class Command {
    public:
        using Action = std::function<std::unique_ptr<CliAction>(config::UserConfig &, ArgMatches &&)>;

    private:
        boost::program_options::options_description options;
        boost::program_options::positional_options_description positionals;
        std::string_view desc;
        Action action;

    public:
        /**
         * Crée une nouvelle commande.
         *
         * @param name Le nom de la commande.
         */
        Command(std::string_view name, std::string_view desc, Action action);

        /**
         * Ajoute un nouveau drapeau aux options valides.
         *
         * Un drapeau n'a pas de valeur associée.
         *
         * @param name Le nom de l'argument.
         * @param description La description de l'argument.
         * @return La commande pour chaîner les appels.
         */
        Command &flag(std::string_view name, std::string_view description);

        /**
         * Ajoute un nouvel argument aux options valides.
         *
         * Un argument contient obligatoirement une valeur.
         *
         * @param name Le nom de l'argument.
         * @param description La description de l'argument.
         * @return La commande pour chaîner les appels.
         */
        Command &arg(std::string_view name, std::string_view description);

        /**
         * Ajoute un nouvel argument optionnel avec une valeur aux options valides.
         *
         * @param name Le nom de l'argument.
         * @param description La description de l'argument.
         * @return La commande pour chaîner les appels.
         */
        template <typename T>
        Command &arg(std::string_view name, std::string_view description) {
            options.add_options()(name.data(), boost::program_options::value<T>(), description.data());
            return *this;
        }

        /**
         * Ajoute un nouvel argument obligatoire avec une valeur aux options valides.
         *
         * @param name Le nom de l'argument.
         * @param description La description de l'argument.
         * @return La commande pour chaîner les appels.
         */
        template <typename T>
        Command &require(std::string_view name, std::string_view description) {
            options.add_options()(name.data(), boost::program_options::value<T>()->required(), description.data());
            return *this;
        }

        /**
         * Rend un argument positionnel.
         *
         * Un argument du même nom doit exister (créé avec arg() ou require()).
         *
         * @param name Le nom de l'argument.
         * @param maxCount Le nombre maximal d'occurrences de l'argument.
         * @return La commande pour chaîner les appels.
         */
        Command &positional(std::string_view name, int maxCount = 1);

        /**
         * Exécute la commande avec les arguments donnés.
         *
         * @param config La configuration de l'utilisateur.
         * @param args Les arguments de la ligne de commande.
         * @return Le code de retour de l'exécution.
         */
        std::unique_ptr<CliAction> create(config::UserConfig &config, std::span<const char *> args) const;

        /**
         * Récupère la description de la commande.
         *
         * @return La description de la commande.
         */
        std::string_view getDescription() const;

        /**
         * Récupère ce qui contient réellement les options de la commande.
         *
         * @return Les options de la commande.
         */
        const boost::program_options::options_description &getBackingOptions() const;

        /**
         * Récupère ce qui contient réellement les options positionnelles de la commande.
         *
         * @return Les options de la commande.
         */
        const boost::program_options::positional_options_description &getBackingPositionalOptions() const;
    };
}
