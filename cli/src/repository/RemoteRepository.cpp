#include <nlohmann/json.hpp>

#include "../io/Archive.h"
#include "../io/HttpRequest.h"
#include "../io/TmpFile.h"
#include "RemoteRepository.h"

using json = nlohmann::json;

namespace repository {
    static io::HttpRequest createRequest(std::string_view url) {
        io::HttpRequest request{url};
        request.addHeader("Accept: application/json");
        request.addHeader("User-Agent: oki/0.1");
        return request;
    }

    static json tryReadRequest(io::HttpRequest &request) {
        io::HttpResponse response = request.get();
        if (!response.getContentType().starts_with("application/json")) {
            throw io::APIException{"Invalid content type received (" + response.getContentType() + ") from " + request.getUrl()};
        }
        json data = json::parse(response.getContent());
        auto it = data.find("error");
        if (response.getStatusCode() >= 400 || it != data.end()) {
            throw io::APIException{(it == data.end() ? "Invalid request" : it->get<std::string>()) + ", tried " + request.getUrl()};
        }
        return data;
    }

    RemoteRepository::RemoteRepository(std::string_view apiUrl) : apiUrl{apiUrl} {}

    std::vector<package::Package> RemoteRepository::listPackages() {
        io::HttpRequest request = createRequest(apiUrl + "/api/list");
        json data = tryReadRequest(request);
        std::vector<package::Package> packages;
        for (const auto &item : data.at("packages")) {
            packages.emplace_back(item.at("name").get<std::string>(), item.at("description").get<std::string>());
        }
        return packages;
    }

    void RemoteRepository::download(const package::DownloadableVersion &packageVersion, const std::filesystem::path &destination) {
        io::HttpRequest request = createRequest(apiUrl + packageVersion.getDownloadUrl());
        io::TmpFile tmp;
        request.download(tmp.getFilename());
        io::Extractor extractor{destination};
        extractor.extract(tmp.getFilename());
    }

    package::Package RemoteRepository::getPackageInfo(std::string_view packageName) {
        io::HttpRequest request = createRequest(apiUrl + "/api/info/" + std::string{packageName});
        json data = tryReadRequest(request);
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

    std::string RemoteRepository::getPackageURL(std::string_view packageName, std::string packageVersion) {
        io::HttpRequest request = createRequest(apiUrl + "/api/version/" + std::string{packageName} + "?version=" + packageVersion);
        json data = tryReadRequest(request);
        return data.get<std::string>();
    }

    void RemoteRepository::publish(config::Manifest &manifest, const std::filesystem::path &source) {
        io::HttpRequest request = createRequest(apiUrl + "/api/publish");
        io::MimePart mime = request.addMime();
        mime.addDataPart("manifest", manifest.asFilteredJson());
        mime.addFilePart("package", source);
        json data = tryReadRequest(request);
    }
}
