#include <nlohmann/json.hpp>

#include "../io/Archive.h"
#include "../io/HttpRequest.h"
#include "../io/TmpFile.h"
#include "RemoteRepository.h"

using json = nlohmann::json;

namespace oki {
    static HttpRequest createRequest(std::string_view url) {
        HttpRequest request{url};
        request.addHeader("Accept: application/json");
        request.addHeader("User-Agent: oki/0.1");
        return request;
    }

    static json tryReadRequest(HttpRequest &request) {
        HttpResponse response = request.get();
        if (!response.getContentType().starts_with("application/json")) {
            throw APIException{"Invalid content type received (" + response.getContentType() + ") from " + request.getUrl()};
        }
        json data = json::parse(response.getContent());
        auto it = data.find("error");
        if (response.getStatusCode() >= 400 || it != data.end()) {
            throw APIException{(it == data.end() ? "Invalid request" : it->get<std::string>()) + ", tried " + request.getUrl()};
        }
        return data;
    }

    RemoteRepository::RemoteRepository(std::string_view apiUrl) : apiUrl{apiUrl} {}

    std::vector<Package> RemoteRepository::listPackages() {
        HttpRequest request = createRequest(apiUrl + "/api/list");
        json data = tryReadRequest(request);
        std::vector<Package> packages;
        for (const auto &item : data.at("packages")) {
            packages.emplace_back(item.at("short_name").get<std::string>(), item.at("description").get<std::string>());
        }
        return packages;
    }

    void RemoteRepository::download(const Version &packageVersion, const std::filesystem::path &destination) {
        HttpRequest request = createRequest(apiUrl + packageVersion.getDownloadUrl());
        TmpFile tmp;
        request.download(tmp.getFilename());
        Extractor extractor{destination};
        extractor.extract(tmp.getFilename());
    }

    Package RemoteRepository::getPackageInfo(std::string_view packageName) {
        HttpRequest request = createRequest(apiUrl + "/api/info/" + std::string{packageName});
        json data = tryReadRequest(request);
        std::vector<Version> versions;
        auto it = data.find("versions");
        if (it != data.end()) {
            for (const auto &item : *it) {
                versions.emplace_back(item.at("identifier").get<std::string>(), item.at("published_date").get<std::string>(), item.at("download_url").get<std::string>());
            }
        }
        return {data.at("short_name").get<std::string>(), data.at("description").get<std::string>(), versions};
    }

    std::string RemoteRepository::getPackageURL(std::string_view packageName, std::string packageVersion) {
        HttpRequest request = createRequest(apiUrl + "/api/version" + std::string{packageName} + "?version=" + packageVersion);
        json data = tryReadRequest(request);
        return data.get<std::string>();
    }

}
