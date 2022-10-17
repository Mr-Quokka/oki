#include <nlohmann/json.hpp>

#include "../io/Archive.h"
#include "../io/HttpRequest.h"
#include "../io/TmpFile.h"
#include "RemoteRepository.h"

using json = nlohmann::json;

namespace oki {
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

    void RemoteRepository::download(const Version &packageVersion, const std::filesystem::path &destination) {
        HttpRequest request{apiUrl + packageVersion.getDownloadUrl()};
        TmpFile tmp;
        request.download(tmp.getFilename());
        Extractor extractor{destination};
        extractor.extract(tmp.getFilename());
    }

    Package RemoteRepository::showPackage(std::string_view packageName) {
        HttpRequest request{apiUrl + "?api=info&package=" + std::string{packageName}};
        json data = json::parse(request.get());
        std::vector<Version> versions;
        if (data.contains("error")) {
            throw APIException(data.at("error").get<std::string>());
        }
        if (data.contains("versions")) {
            for (const auto &item : data.at("versions")) {
                versions.emplace_back(item.at("identifier").get<std::string>(), item.at("published_date").get<std::string>(), item.at("download_url").get<std::string>());
            }
        }
        return Package(data.at("short_name").get<std::string>(), data.at("long_name").get<std::string>(), versions);
    }
}
