#pragma once

#include <string>

namespace io {
    /**
     * Quelque chose de téléchargable par HTTP ou un autre moyen.
     */
    class Downloadable {
    private:
        std::string downloadUrl;

    public:
        explicit Downloadable(std::string_view downloadUrl);
        const std::string &getDownloadUrl() const;
    };
}
