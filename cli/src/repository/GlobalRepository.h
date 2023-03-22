#pragma once

#include "Repository.h"

namespace repository {
    struct RepositoryId {
        std::string name;
        std::unique_ptr<repository::Repository> repository;
    };

    /**
     * Un dépôt de paquets redirigeant vers d'autres dépôts.
     */
    class GlobalRepository : public Repository {
    private:
        std::vector<RepositoryId> sources;

    public:
        /**
         * Récupère un dépôt par son nom.
         *
         * @param name Le nom du dépôt.
         * @return Le dépôt.
         * @throws std::invalid_argument Si le dépôt n'existe pas.
         */
        repository::Repository &getRepository(std::string_view name);

        /**
         * Récupère un dépôt par son nom.
         *
         * @param name Le nom du dépôt.
         * @return Le dépôt.
         * @throws std::invalid_argument Si le dépôt n'existe pas.
         */
        const repository::Repository &getRepository(std::string_view name) const;

        /**
         * Ajoute une nouvelle source.
         *
         * Si une source avec le même nom existe déjà, alors elle est conservée et la nouvelle n'est pas ajoutée.
         *
         * @param name Le nom de la source.
         * @param repository Le dépôt.
         * @return Si la source a été ajoutée.
         */
        bool addSource(std::string_view name, std::unique_ptr<repository::Repository> &&repository);

        /**
         * Ajoute les dépôts par défaut s'ils n'y sont pas déjà.
         */
        void addDefaultSources();

        /**
         * Liste les paquets disponibles dans le dépôt principal, c'est-à-dire le premier.
         *
         * @return Une liste de paquets.
         */
        std::vector<package::Package> listPackages(SearchParameters &params) override;

        /**
         * Récupère les informations d'un paquet en demandant à chaque dépôt.
         *
         * @param packageName Le nom du paquet.
         * @return Les informations du paquet.
         */
        package::Package getPackageInfo(std::string_view packageName) override;

        /**
         * Récupère les informations d'un paquet en ayant pour préférence un dépôt.
         *
         * @param packageName Le nom du paquet.
         * @param preferredRegistry Le dépôt préféré à utiliser (ou nullptr).
         * @return Les informations du paquet.
         */
        std::pair<package::Package, std::reference_wrapper<Repository>> getPackageInfo(std::string_view packageName, Repository *preferredRegistry);

        bool publish(config::Manifest &manifest, const std::filesystem::path &source) override;
    };
}
