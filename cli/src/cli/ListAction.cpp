#include "ListAction.h"

#include <iostream>

namespace cli {
    void ListAction::run(repository::Repository &repository) {
        for (const package::Package &package : repository.listPackages()) {
            std::cout << package.getShortName() << ": " << package.getShortName() << "\n";
        }
    }
}
