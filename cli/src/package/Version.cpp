#include "Version.h"

namespace oki{
	Version::Version(std::string_view identifier, std::string_view publishedDate): identifier{identifier}, publishedDate{publishedDate}{}

	const std::string& Version::getIdentifier() const{
		return this->identifier;
	}
    const std::string& Version::getPublishedDate() const{
    	return this->publishedDate;
    }
}