#pragma once

#include <string>

namespace config {
    class AuthProvider {
    public:
        std::pair<std::string, std::string> get();
    };
}
