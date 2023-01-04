#pragma once

#include <iterator>
#include <ostream>

#define EXPERIMENTAL_ITERATOR
#ifdef EXPERIMENTAL_ITERATOR
#include <experimental/iterator>
#endif

namespace util {
    /**
     * Rejoint un itérateur avec entre chaque élément un délimiteur donné.
     *
     * @tparam Iterator le type de l'itérateur
     * @tparam IteratorSentinel le type de fin de l'itérateur
     * @param os le flux de sortie
     * @param it l'itérateur à partir duquel commencer
     * @param end l'itérateur marquant la fin
     * @param delimiter le délimiteur à utiliser entre deux éléments
     * @return le flux de sortie
     */
    template <std::input_iterator Iterator, std::sentinel_for<Iterator> IteratorSentinel>
    std::ostream &ostreamJoin(std::ostream &os, Iterator it, IteratorSentinel end, char const *delimiter, char const *before = "\0") {
#ifdef EXPERIMENTAL_ITERATOR
        if (it != end) {
            os << before;
        }
        std::copy(it, end, std::experimental::make_ostream_joiner(os, delimiter));
#else
        if (it != end) {
            os << before << *it;
            ++it;
        }
        for (; it != end; ++it) {
            os << delimiter << *it;
        }
#endif
        return os;
    }
}