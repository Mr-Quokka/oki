#pragma once

#include "../config/UserConfig.h"
#include "../repository/Repository.h"
#include <boost/program_options/variables_map.hpp>
#include <optional>
#include <span>

namespace cli {
    class Command;

    /**
     * Un conteneur pour des arguments lus depuis la ligne de commande.
     */
    class ArgMatches {
    private:
        boost::program_options::variables_map vm;

    public:
        /**
         * Tente de lire les arguments depuis la ligne de commande.
         *
         * @param args Les arguments de la ligne de commande.
         * @param command La commande à utiliser.
         */
        ArgMatches(std::span<const char *> args, const Command &command);

        ArgMatches(boost::program_options::variables_map &&vm); // NOLINT(google-explicit-constructor)

        /**
         * Teste si un argument ou un drapeau a été fourni.
         *
         * @param name Le nom de l'argument ou du drapeau.
         * @return true s'il a été fourni, false sinon.
         */
        bool contains(const std::string &name) const;

        /**
         * Récupère la valeur d'un argument.
         *
         * Un optional vide est retourné si l'argument n'a pas été fourni.
         *
         * @tparam T Le type de l'argument.
         * @param key Le nom de l'argument.
         * @return La valeur de l'argument.
         */
        template <typename T>
        std::optional<T> get(const std::string &key) const {
            if (vm.count(key) == 0) {
                return std::nullopt;
            }
            return vm[key].as<T>();
        }

        /**
         * Récupère la valeur d'un argument.
         *
         * Une exception est levée si l'argument n'a pas été fourni.
         *
         * @tparam T Le type de l'argument.
         * @param key Le nom de l'argument.
         * @return La valeur de l'argument.
         */
        template <typename T>
        T require(const std::string &key) const {
            auto it = vm.find(key);
            if (it == vm.end()) {
                throw std::out_of_range("Missing required argument: " + key);
            }
            return it->second.as<T>();
        }

        /**
         * Récupère le registre à utiliser d'après les arguments.
         *
         * @return Le registre à utiliser.
         */
        repository::Repository &getRegistry(config::UserConfig &config);
    };
}
