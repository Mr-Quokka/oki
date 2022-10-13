#pragma once

namespace oki{
    class TmpFile {
    private:
        char filename[20];
        int fd;
    public:
        TmpFile();
        const char *getFilename();
        ~TmpFile();
    };
}
