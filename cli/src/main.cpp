#include <iostream>

#include "config/config.h"
#include "repository/LocalRepository.h"

int main() {
	LocalRepository repo{getDefaultLocalRepository()};
	repo.createIfNotExists();
	for (const Package& package : repo.listPackages()) {
		std::cout << package.getShortName() << "\n";
	}
	return 0;
}
