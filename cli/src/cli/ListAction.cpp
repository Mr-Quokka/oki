#include "ListAction.h"

#include <iostream>

namespace oki {
    ListAction::ListAction() {}

    void ListAction::run(Repository &repository) {
        for (const Package &package : repository.listPackages()) {
            std::cout << package.getShortName() << ": " << package.getLongName() << "\n";
        }
    }
}
