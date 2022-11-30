#include "ParseException.h"

#include <iterator>

namespace semver {
    ParseException::ParseException(const char *message, std::string_view parsed,
                                   std::size_t offset)
        : runtime_error{message}, parsed{parsed}, offset{offset} {}

    ParseException::ParseException(const char *message, std::string_view parsed, std::ptrdiff_t offset)
        : ParseException{message, parsed, static_cast<std::size_t>(offset)} {}

    std::size_t ParseException::getOffset() const {
        return offset;
    }

    void ParseException::addContext(std::string_view message) {
        messageStack.emplace_back(message);
    }

    std::ostream &operator<<(std::ostream &out, const ParseException &exception) {
        auto it = exception.messageStack.cbegin();
        while (it != exception.messageStack.cend()) {
            out << *it << "\n";
            ++it;
            out << "Caused by:\n  ";
        }

        std::size_t start = exception.offset - std::min(exception.offset, ParseException::ExtractLen);
        std::size_t len = std::min(exception.parsed.length(), exception.offset + ParseException::ExtractLen);
        std::string_view extract = std::string_view{exception.parsed}.substr(start, len);
        out << extract << "\n";
        std::size_t off = exception.messageStack.empty() ? 0 : 2;
        std::fill_n(std::ostream_iterator<char>(out), off + exception.offset - start, ' ');
        out << "^\n";
        out << exception.what();
        return out;
    }
}
