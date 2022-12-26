#include "TmpFile.h"

#include <cstdlib>
#include <system_error>
#include <unistd.h>

namespace fs = std::filesystem;

namespace io {
    TmpFile::TmpFile() : filename{"/tmp/oki-XXXXXX"}, fd{mkstemp(this->filename)}, filePath{this->filename} {
        if (fd == -1) {
            throw std::system_error{errno, std::generic_category()};
        }
    }

    const fs::path &TmpFile::path() const {
        return filePath;
    }

    TmpFile::~TmpFile() {
        unlink(filename);
    }

    TmpDir::TmpDir() : dirname{"/tmp/oki-XXXXXX"} {
        if (mkdtemp(dirname) == nullptr) {
            throw std::system_error{errno, std::generic_category()};
        }
        dirPath = fs::path{dirname};
    }

    const fs::path &TmpDir::path() const {
        return dirPath;
    }

    TmpDir::~TmpDir() {
        fs::remove_all(dirPath);
    }
}
