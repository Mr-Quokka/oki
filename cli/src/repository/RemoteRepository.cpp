#include <nlohmann/json.hpp>

#include "RemoteRepository.h"
#include "../io/HttpRequest.h"

using json = nlohmann::json;

namespace oki{
    RemoteRepository::RemoteRepository(std::string_view apiUrl) : apiUrl{apiUrl} {}

    std::vector<Package> RemoteRepository::listPackages() {
        HttpRequest request{apiUrl + "?api=list"};
        json data = json::parse(request.get());
        std::vector<Package> packages;
        for (const auto &item : data) {
            packages.emplace_back(item.at("short_name").get<std::string>(), item.at("long_name").get<std::string>());
        }
    	return packages;
    }

    void RemoteRepository::download(std::string_view packageName, const std::filesystem::path &destination) {

    }

    std::optional<Package> RemoteRepository::showPackage(std::string_view packageName){
        HttpRequest request{apiUrl + "?api=info&package=" + std::string{packageName}};
        json data = json::parse(request.get());
        std::vector<Version> versions;
        for (const auto &item : data.at("versions")){
            versions.emplace_back(item.at("identifier").get<std::string>(), item.at("published_date").get<std::string>());
        }
        return Package(data.at("short_name").get<std::string>(),data.at("long_name").get<std::string>(), versions);
    }
}
