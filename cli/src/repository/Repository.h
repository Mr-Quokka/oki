#pragma once

#include <filesystem>
#include <optional>
#include <string_view>
#include <vector>

#include "../config/Manifest.h"
#include "../package/Package.h"
#include "SearchParameters.h"

namespace repository {
    /**
     * Un dépôt où sont référencés des paquets.
     */
    class Repository {
    public:
        /**
         * Liste tous les paquets présents sur le dépôt.
         *
         * @param params Les paramètres de recherche à utiliser.
         * @return Tous les noms des paquets disponibles.
         */
        virtual std::vector<package::Package> listPackages(SearchParameters &params) = 0;

        /**
         * Récupère les informations détaillées d'un paquet nommé.
         *
         * @param packageName Le nom du paquet à utiliser.
         * @return Les informations de ce paquet.
         */
        virtual package::Package getPackageInfo(std::string_view packageName) = 0;
        virtual bool publish(config::Manifest &manifest, const std::filesystem::path &source) = 0;
        virtual ~Repository() = default;
    };
}
