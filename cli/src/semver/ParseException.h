#pragma once

#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace semver {
    class ParseException : public std::runtime_error {
    private:
        std::string parsed;
        std::size_t offset;
        std::vector<std::string> messageStack;

    public:
        static constexpr std::size_t ExtractLen = 20;
        ParseException(const char *message, std::string_view parsed, std::size_t offset);
        ParseException(const char *message, std::string_view parsed, std::ptrdiff_t offset);
        void addContext(std::string_view message);
        std::size_t getOffset() const;
        friend std::ostream &operator<<(std::ostream &out, const ParseException &);
    };

    std::ostream &operator<<(std::ostream &out, const ParseException &exception);
}
