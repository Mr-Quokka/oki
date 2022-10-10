#pragma once

#include <string>
#include <vector>

namespace oki{
    class Version {
    private:
        std::string identifier;
        std::string published_date;
    public:
        Version(std::string_view identifier, std::string_view published_date);
        const std::string& getIdentifier() const;
        const std::string& getPublishedDate() const;
    };
}