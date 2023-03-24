#include "Checksum.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/sha.h>

#define BUF_LEN 32768

namespace io {
    Sha256Checksum::Sha256Checksum(const std::array<uint8_t, 32> &array)
        : std::array<uint8_t, 32>{array} {}

    Sha256Checksum::Sha256Checksum(std::string_view str) {
        if (str.size() != SHA256_DIGEST_LENGTH * 2) {
            throw std::invalid_argument{"Invalid checksum"};
        }

        for (size_t i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            char buf[3];
            buf[0] = str[i * 2];
            buf[1] = str[i * 2 + 1];
            buf[2] = '\0';
            (*this)[i] = static_cast<uint8_t>(std::strtol(buf, nullptr, 16));
        }
    }

    Sha256Checksum Sha256Checksum::ofFile(const std::filesystem::path &path) {
        std::ifstream file{path, std::ios::binary};
        if (!file) {
            throw std::runtime_error{"Unable to open file"};
        }

        // Définit la fonction de hash
        EVP_MD_CTX *digestCtx = EVP_MD_CTX_new();
        const EVP_MD *digestType = EVP_sha256();
        EVP_DigestInit_ex(digestCtx, digestType, nullptr);

        // Détermine le hash au fur et à mesure de la lecture
        {
            auto chunk = std::make_unique<char[]>(BUF_LEN);
            while (file) {
                file.read(chunk.get(), BUF_LEN);
                EVP_DigestUpdate(digestCtx, chunk.get(), static_cast<size_t>(file.gcount()));
            }
        }

        // Récupère le hash sur 256 bits
        Sha256Checksum checksum;
        static_assert(std::size(checksum) == SHA256_DIGEST_LENGTH);
        EVP_DigestFinal_ex(digestCtx, checksum.data(), nullptr);
        EVP_MD_CTX_free(digestCtx);
        return checksum;
    }

    std::ostream &operator<<(std::ostream &out, const Sha256Checksum &checksum) {
        out << std::hex << std::setfill('0');
        for (unsigned char c : checksum) {
            out << std::setw(2) << static_cast<int>(c);
        }
        return out;
    }

    std::string Sha256Checksum::str() const {
        std::ostringstream out;
        out << *this;
        return out.str();
    }
}
