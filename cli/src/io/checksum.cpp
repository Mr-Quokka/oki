#include "checksum.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/sha.h>

#define BUF_LEN 32768

namespace io {
    Sha256Checksum computeSha256(const std::filesystem::path &path) {
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

    std::string checksumAsString(const Checksum &checksum) {
        std::ostringstream out;
        out << std::hex << std::setfill('0');
        for (unsigned char c : checksum) {
            out << std::setw(2) << static_cast<int>(c);
        }
        return out.str();
    }
}
