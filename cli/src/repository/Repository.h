#pragma once

#include <filesystem>
#include <optional>
#include <string_view>
#include <vector>

#include "../config/Manifest.h"
#include "../package/Package.h"

namespace repository {
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
        virtual std::vector<package::Package> listPackages() = 0;

        /**
         * Récupère les informations détaillées d'un paquet nommé.
         *
         * @param packageName Le nom du paquet à utiliser.
         * @return Les informations de ce paquet.
         */
        virtual package::Package getPackageInfo(std::string_view packageName) = 0;
        virtual std::string getPackageURL(std::string_view packageName, std::string packageVersion) = 0;
        virtual void download(const package::DownloadableVersion &packageVersion, const std::filesystem::path &destination) = 0;
        virtual void publish(config::Manifest &manifest, const std::filesystem::path &source) = 0;
        virtual ~Repository() = default;
    };
}
