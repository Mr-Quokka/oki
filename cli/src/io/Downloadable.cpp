#include "Downloadable.h"

namespace io {
    Downloadable::Downloadable(std::string_view downloadUrl)
        : downloadUrl{downloadUrl} {}

    const std::string &Downloadable::getDownloadUrl() const {
        return downloadUrl;
    }
}
