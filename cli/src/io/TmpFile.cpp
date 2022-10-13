#include "TmpFile.h"

#include <cstdlib>
#include <unistd.h>

namespace oki{
    TmpFile::TmpFile() : filename{"/tmp/oki-XXXXXX"}, fd{mkstemp(this->filename)} {}

    const char *TmpFile::getFilename() {
        return filename;
    }

    TmpFile::~TmpFile() {
        unlink(filename);
    }
}
