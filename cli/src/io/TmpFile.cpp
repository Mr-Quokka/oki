#include "TmpFile.h"

#include <cstdlib>
#include <system_error>
#include <unistd.h>

namespace io {
    TmpFile::TmpFile() : filename{"/tmp/oki-XXXXXX"}, fd{mkstemp(this->filename)} {
        if (fd == -1) {
            throw std::system_error{errno, std::generic_category()};
        }
    }

    const char *TmpFile::getFilename() const {
        return filename;
    }

    TmpFile::~TmpFile() {
        unlink(filename);
    }

    TmpDir::TmpDir() : dirname{"/tmp/oki-XXXXXX"} {
        if (mkdtemp(dirname) == nullptr) {
            throw std::system_error{errno, std::generic_category()};
        }
    }

    const char *TmpDir::getDirname() const {
        return dirname;
    }

    TmpDir::~TmpDir() {
        unlink(dirname);
    }
}
