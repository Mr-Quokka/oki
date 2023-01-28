#pragma once

#include <stdexcept>

namespace repository {
    class RepositoryException : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}
