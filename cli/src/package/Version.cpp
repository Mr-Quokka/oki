#include "Version.h"

namespace oki{
	Version::Version(std::string_view identifier, std::string_view published_date): identifier{identifier}, published_date{published_date}{}

	const std::string& Version::getIdentifier() const{
		return this->identifier;
	}
    const std::string& Version::getPublishedDate() const{
    	return this->published_date;
    }
}