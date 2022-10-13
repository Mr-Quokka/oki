#include "Version.h"

namespace oki{
    Version::Version(std::string_view identifier, std::string_view publishedDate, std::string_view downloadUrl)
            : identifier{identifier}, publishedDate{publishedDate}, downloadUrl{downloadUrl} {}

	const std::string& Version::getIdentifier() const{
		return this->identifier;
	}
    const std::string& Version::getPublishedDate() const{
    	return this->publishedDate;
    }
    const std::string& Version::getDownloadUrl() const{
        return this->downloadUrl;
    }
}