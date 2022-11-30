#include "Archive.h"

#include <minizip/unzip.h>
#include <sys/stat.h>

#include <utility>

#define BUFFER_SIZE 256
#define DIRECTORY_PERMISSIONS 755

// Basé sur https://github.com/madler/zlib/blob/master/contrib/minizip/miniunz.c

namespace io {
    Extractor::Extractor(std::filesystem::path destination) : destination{std::move(destination)} {}

    void Extractor::extract(const std::filesystem::path &archivePath) {
        unzFile zipFile = unzOpen(archivePath.c_str());
        if (zipFile == nullptr) {
            return;
        }
        unz_global_info globalInfo;
        if (unzGetGlobalInfo(zipFile, &globalInfo) != UNZ_OK) {
            unzClose(zipFile);
            return;
        }
        char readBuf[BUFFER_SIZE], filenameBuf[BUFFER_SIZE + 1] = {0};
        for (uLong i = 0; i < globalInfo.number_entry; ++i) {
            unz_file_info fileInfo;
            if (unzGetCurrentFileInfo(zipFile, &fileInfo, filenameBuf, BUFFER_SIZE, nullptr, 0, nullptr, 0) != UNZ_OK) {
                unzClose(zipFile);
                return;
            }
            std::string_view filename = filenameBuf;
            if (filename.back() == '/') {
                mkdir((destination / filename).c_str(), DIRECTORY_PERMISSIONS);
            } else {
                if (unzOpenCurrentFile(zipFile) != UNZ_OK) {
                    unzClose(zipFile);
                    return;
                }

                FILE *out = fopen((destination / filename).c_str(), "wb");
                if (out == nullptr) {
                    unzCloseCurrentFile(zipFile);
                    unzClose(zipFile);
                    return;
                }

                int error = UNZ_OK;
                do {
                    error = unzReadCurrentFile(zipFile, readBuf, BUFFER_SIZE);
                    if (error < 0) {
                        unzCloseCurrentFile(zipFile);
                        unzClose(zipFile);
                        return;
                    }

                    if (error > 0) {
                        fwrite(readBuf, error, 1, out);
                    }
                } while (error > 0);

                fclose(out);
            }
            unzCloseCurrentFile(zipFile);

            if ((i + 1) < globalInfo.number_entry) {
                if (unzGoToNextFile(zipFile) != UNZ_OK) {
                    unzClose(zipFile);
                    return;
                }
            }
        }
        unzClose(zipFile);
    }
}
