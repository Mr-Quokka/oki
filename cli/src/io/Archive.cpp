#include "Archive.h"

#include <minizip/unzip.h>
#include <minizip/zip.h>
#include <sys/stat.h>

#include <fstream>
#include <utility>
#include <vector>

#define BUFFER_SIZE 256

// Basé sur https://github.com/madler/zlib/blob/master/contrib/minizip/miniunz.c
// et https://github.com/madler/zlib/blob/master/contrib/minizip/minizip.c

namespace fs = std::filesystem;

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
            fs::path path = destination / filename;
            if (filename.back() == '/') {
                fs::create_directories(path);
            } else {
                if (unzOpenCurrentFile(zipFile) != UNZ_OK) {
                    unzClose(zipFile);
                    return;
                }

                FILE *out = fopen(path.c_str(), "wb");
                if (out == nullptr) {
                    // Le dossier qui contient le fichier n'existe pas
                    fs::create_directories(path.parent_path());
                    out = fopen(path.c_str(), "wb");
                }
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

    Compressor::Compressor(fs::path destination)
        : destination{std::move(destination)} {}

    void Compressor::compress(std::span<const fs::path> files, const std::filesystem::path &relativeTo) {
        unzFile zipFile = zipOpen(destination.c_str(), APPEND_STATUS_CREATE);
        if (zipFile == nullptr) {
            return;
        }
        std::vector<char> buffer;
        zip_fileinfo fileInfo;
        struct stat s;
        struct tm *filedate;
        for (const auto &path : files) {
            std::ifstream file{path, std::ios::binary};
            if (!file.is_open()) {
                continue;
            }
            if (stat(path.c_str(), &s) == -1 || (filedate = localtime(&s.st_mtim.tv_sec)) == nullptr) {
                continue;
            }
            fileInfo.tmz_date.tm_sec = static_cast<uInt>(filedate->tm_sec);
            fileInfo.tmz_date.tm_min = static_cast<uInt>(filedate->tm_min);
            fileInfo.tmz_date.tm_hour = static_cast<uInt>(filedate->tm_hour);
            fileInfo.tmz_date.tm_mday = static_cast<uInt>(filedate->tm_mday);
            fileInfo.tmz_date.tm_mon = static_cast<uInt>(filedate->tm_mon);
            fileInfo.tmz_date.tm_year = static_cast<uInt>(filedate->tm_year);

            buffer.reserve(static_cast<std::size_t>(s.st_size));
            if (file.read(buffer.data(), s.st_size)) {
                fs::path relative = fs::relative(path, relativeTo);
                if (zipOpenNewFileInZip(zipFile, relative.c_str(), &fileInfo, nullptr, 0, nullptr, 0, nullptr, Z_DEFLATED, Z_DEFAULT_COMPRESSION) == ZIP_OK) {
                    zipWriteInFileInZip(zipFile, buffer.data(), static_cast<uInt>(s.st_size));
                    zipCloseFileInZip(zipFile);
                }
            }
        }
        zipClose(zipFile, nullptr);
    }
}
