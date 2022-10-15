#pragma once

#include <filesystem>
#include <optional>
#include <string_view>
#include <vector>

#include "../package/Package.h"

namespace oki {
    /**
     * Un dépôt où sont référencés des paquets.
     */
    class Repository {
    public:
        /**
         * Liste tous les paquets présents sur le dépôt.
         *
         * @return Tous les noms des paquets disponibles.
         */
        virtual std::vector<Package> listPackages() = 0;

        /**
         * Récupère les informations détaillées d'un paquet nommé.
         *
         * @param packageName Le nom du paquet à utiliser.
         * @return Les informations de ce paquet.
         */
        virtual std::optional<Package> showPackage(std::string_view packageName) = 0;
        virtual void download(const Version &packageVersion, const std::filesystem::path &destination) = 0;
        virtual ~Repository() = default;
    };
}
