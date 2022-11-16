#include <nlohmann/json.hpp>

#include "../io/Archive.h"
#include "../io/HttpRequest.h"
#include "../io/TmpFile.h"
#include "RemoteRepository.h"

using json = nlohmann::json;

namespace repository {
    RemoteRepository::RemoteRepository(std::string_view apiUrl) : apiUrl{apiUrl} {}

    std::vector<package::Package> RemoteRepository::listPackages() {
        io::HttpRequest request{apiUrl + "?api=list"};
        json data = json::parse(request.get());
        std::vector<package::Package> packages;
        for (const auto &item : data) {
            packages.emplace_back(item.at("short_name").get<std::string>(), item.at("description").get<std::string>());
        }
        return packages;
    }

    void RemoteRepository::download(const package::Version &packageVersion, const std::filesystem::path &destination) {
        io::HttpRequest request{apiUrl + packageVersion.getDownloadUrl()};
        io::TmpFile tmp;
        request.download(tmp.getFilename());
        io::Extractor extractor{destination};
        extractor.extract(tmp.getFilename());
    }

    std::optional<package::Package> RemoteRepository::showPackage(std::string_view packageName) {
        io::HttpRequest request{apiUrl + "?api=info&package=" + std::string{packageName}};
        json data = json::parse(request.get());
        std::vector<package::Version> versions;
        if (data.contains("error")) {
            throw io::APIException(data.at("error").get<std::string>());
        }
        if (data.contains("versions")) {
            for (const auto &item : data.at("versions")) {
                versions.emplace_back(item.at("identifier").get<std::string>(), item.at("published_date").get<std::string>(), item.at("download_url").get<std::string>());
            }
        }
        return package::Package{data.at("short_name").get<std::string>(), data.at("description").get<std::string>(), versions};
    }

    std::string RemoteRepository::getPackageURL(std::string_view packageName, std::string packageVersion) {
        io::HttpRequest request{apiUrl + "?api=version&name=" + std::string{packageName} + "&version=" + packageVersion};
        json data = json::parse(request.get());
        if (data.contains("error")) {
            throw io::APIException(data.at("error").get<std::string>());
        } else
            return data.get<std::string>();
    }

}
