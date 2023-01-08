#include "AuthProvider.h"

#include <iostream>
#include <termios.h>
#include <unistd.h>

namespace config {
    std::pair<std::string, std::string> AuthProvider::get() {
        const char *username = std::getenv("OKI_USERNAME");
        const char *password = std::getenv("OKI_PASSWORD");
        if (username != nullptr && password != nullptr) {
            return std::make_pair(username, password);
        }
        return read();
    }

    std::pair<std::string, std::string> AuthProvider::read() {
        std::cout << "Username: ";
        std::string username;
        std::getline(std::cin, username);

        std::cout << "Password: ";
        std::string password;
        struct termios tty;
        tcgetattr(STDIN_FILENO, &tty);
        tty.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &tty);
        std::cin >> password;
        tty.c_lflag |= ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &tty);
        return std::make_pair(std::move(username), std::move(password));
    }
}
