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
        std::vector<package::Package> listPackages() override;
        package::Package getPackageInfo(std::string_view packageName) override;
        void publish(config::Manifest &manifest, const std::filesystem::path &source) override;
    };
}
