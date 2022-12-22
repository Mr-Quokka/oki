#include "RemoveAction.h"
#include "io/oki.h"

#include <iostream>

namespace fs = std::filesystem;

namespace cli {
    RemoveAction::RemoveAction(const char *packageName) : packageName{packageName} {}

    void RemoveAction::run(repository::Repository &repository) {
        bool succes = fs::remove_all(OKI_PACKAGES_DIRECTORY/packageName);
        if(succes){
            std::cout << "The package " << this->packageName << " has been removed successfully !\n";
        } else {
            std::cout << "An error occured while removing the package " << this->packageName << "\n";
            exit(1);
        }
    }

}