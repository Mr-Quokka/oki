#include <nlohmann/json.hpp>

#include "../io/HttpRequest.h"
#include "RemoteRepository.h"
#include "RepositoryException.h"

using json = nlohmann::json;

namespace repository {
    static json tryReadRequest(config::AuthProvider &auth, io::HttpRequest &request) {
        io::HttpResponse response = request.get();
        if (response.requiresAuth()) {
            if (response.requiresBasicAuth()) {
                auto [username, password] = auth.get();
                request.authenticate(io::HttpAuth::Basic, username, password);
                response = request.get();
            }
        }
        if (!response.getContentType().starts_with("application/json")) {
            throw RepositoryException{"Invalid content type received (" + response.getContentType() + ") from " + request.getUrl() + " (" + std::to_string(response.getStatusCode().getCode()) + ")"};
        }
        json data = json::parse(response.getContent());
        auto it = data.find("error");
        if (response.getStatusCode().isError() || it != data.end()) {
            throw RepositoryException{(it == data.end() ? "Invalid request" : it->get<std::string>()) + ", tried " + request.getUrl() + " (" + std::to_string(response.getStatusCode().getCode()) + ")"};
        }
        return data;
    }

    RemoteRepository::RemoteRepository(std::string_view apiUrl) : apiUrl{apiUrl} {}

    std::vector<package::Package> RemoteRepository::listPackages() {
        io::HttpRequest request = io::HttpRequest::createJson(apiUrl + "/api/list");
        json data = tryReadRequest(authProvider, request);
        std::vector<package::Package> packages;
        for (const auto &item : data.at("packages")) {
            packages.emplace_back(item.at("name").get<std::string>(), item.at("description").get<std::string>());
        }
        return packages;
    }

    package::Package RemoteRepository::getPackageInfo(std::string_view packageName) {
        io::HttpRequest request = io::HttpRequest::createJson(apiUrl + "/api/info/" + std::string{packageName});
        json data = tryReadRequest(authProvider, request);
        std::vector<package::PackageVersion> versions;
        auto it = data.find("versions");
        if (it != data.end()) {
            for (const auto &item : *it) {
                versions.emplace_back(
                    semver::Version::parse(item.at("identifier").get<std::string>()),
                    item.at("published_date").get<std::string>(),
                    item.at("download_url").get<std::string>());
                for (auto itDep = item["dependencies"].cbegin(); itDep != item["dependencies"].cend(); ++itDep) {
                    const std::string &keyString = itDep.key();
                    std::string valueString = itDep.value().get<std::string>();
                    versions.back().addDependency(keyString, semver::Range::parse(valueString));
                }
            }
        }
        return {data.at("name").get<std::string>(), data.at("description").get<std::string>(), versions};
    }

    bool RemoteRepository::publish(config::Manifest &manifest, const std::filesystem::path &source) {
        io::HttpRequest request = io::HttpRequest::createJson(apiUrl + "/api/publish");
        io::MimePart mime = request.addMime();
        mime.addDataPart("manifest", manifest.asFilteredJson());
        mime.addFilePart("package", source);
        json data = tryReadRequest(authProvider, request);
        return true;
    }
}
