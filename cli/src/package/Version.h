#pragma once

#include <string>
#include <vector>

namespace oki{
    class Version {
    private:
        std::string identifier;
        std::string publishedDate;
        std::string downloadUrl;
    public:
        Version(std::string_view identifier, std::string_view publishedDate, std::string_view downloadUrl);
        const std::string& getIdentifier() const;
        const std::string& getPublishedDate() const;
        const std::string& getDownloadUrl() const;
    };
}