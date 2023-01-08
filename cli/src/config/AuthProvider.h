#pragma once

#include <string>

namespace config {
    class AuthProvider {
    private:
        std::pair<std::string, std::string> read();

    public:
        std::pair<std::string, std::string> get();
    };
}
