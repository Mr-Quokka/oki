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

    std::ostream &operator<<(std::ostream &out, const ParseException &exception) {
        std::size_t start = exception.offset - std::min(exception.offset, ParseException::ExtractLen);
        std::size_t len = std::min(exception.parsed.length(), exception.offset + ParseException::ExtractLen);
        std::string_view extract = std::string_view{exception.parsed}.substr(start, len);
        out << extract << "\n";
        std::fill_n(std::ostream_iterator<char>(out), exception.offset - start, ' ');
        out << "^\n";
        out << exception.what();
        return out;
    }
}
