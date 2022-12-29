#include "TmpFile.h"

#include <cstdlib>
#include <cstring>
#include <system_error>
#include <unistd.h>

namespace fs = std::filesystem;

namespace io {
    TmpFile::TmpFile() : filename{"/tmp/oki-XXXXXX"}, fd{mkstemp(this->filename)}, filePath{this->filename} {
        if (fd == -1) {
            throw std::system_error{errno, std::generic_category()};
        }
    }

    TmpFile::TmpFile(TmpFile &&other) noexcept : fd{std::exchange(other.fd, -1)}, filePath{std::move(other.filePath)} {
        std::memcpy(filename, other.filename, sizeof(TmpFile::filename));
    }

    TmpFile &TmpFile::operator=(TmpFile &&other) noexcept {
        if (this == &other) {
            return *this;
        }
        std::memcpy(filename, other.filename, sizeof(TmpFile::filename));
        fd = std::exchange(other.fd, -1);
        filePath = std::move(other.filePath);
        return *this;
    }

    const fs::path &TmpFile::path() const {
        return filePath;
    }

    TmpFile::~TmpFile() {
        if (fd != -1) {
            unlink(filename);
        }
    }

    TmpDir::TmpDir() : dirname{"/tmp/oki-XXXXXX"} {
        if (mkdtemp(dirname) == nullptr) {
            throw std::system_error{errno, std::generic_category()};
        }
        dirPath = fs::path{dirname};
    }

    TmpDir::TmpDir(TmpDir &&other) noexcept : dirPath{std::move(other.dirPath)} {
        std::memcpy(dirname, other.dirname, sizeof(TmpDir::dirname));
        *other.dirname = '\0';
    }

    TmpDir &TmpDir::operator=(TmpDir &&other) noexcept {
        if (this == &other) {
            return *this;
        }
        std::memcpy(dirname, other.dirname, sizeof(TmpDir::dirname));
        *other.dirname = '\0';
        dirPath = std::move(other.dirPath);
        return *this;
    }

    const fs::path &TmpDir::path() const {
        return dirPath;
    }

    TmpDir::~TmpDir() {
        if (*dirname) {
            fs::remove_all(dirPath);
        }
    }
}
