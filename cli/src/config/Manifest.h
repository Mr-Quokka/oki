#pragma once

#include <filesystem>
#include <string>
#include <toml.hpp>
#include <unordered_map>

namespace oki {
    class Manifest {
    private:
        toml::v3::table table;
        toml::v3::table &addDependencySectionIfNotExists();

    public:
        explicit Manifest(std::filesystem::path fileName);
        std::unordered_map<std::string_view, std::string> listDeclaredPackages() const;
        void addDeclaredPackage(std::string_view packageName, std::string_view version);
        friend std::ostream &operator<<(std::ostream &, const Manifest &);
    };
    std::ostream &operator<<(std::ostream &os, const Manifest &manifest);
}
