#pragma once

#include "Repository.h"

class RemoteRepository : public Repository {
private:
	std::string apiUrl;
public:
	explicit RemoteRepository(std::string_view apiUrl);
	std::vector<Package> listPackages() override;
	void download(std::string_view packageName, const std::filesystem::path& destination) override;
};
