#include "AuthProvider.h"

#include <iostream>
#include <unistd.h>

namespace config {
    std::pair<std::string, std::string> AuthProvider::get() {
        std::cout << "Username: ";
        std::string username;
        std::getline(std::cin, username);
        std::string password{getpass("Password: ")};
        return std::make_pair(std::move(username), std::move(password));
    }
}
