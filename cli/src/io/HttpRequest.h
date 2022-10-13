#pragma once

#include <filesystem>
#include <string>

namespace oki{
    class HttpRequest {
    private:
        void *curl;
        std::string url;
    public:
        explicit HttpRequest(std::string_view url);
        std::string get();
        void download(const std::filesystem::path& path);
        ~HttpRequest();
    };

    class RequestException : public std::exception {
    private:
        int code;
    public:
        explicit RequestException(int code);
        const char *what() const noexcept override;
    };

    class APIException : public std::exception {
    private:
        std::string msg;
    public:
        explicit APIException(std::string_view msg);
        const char *what() const noexcept override;
    };
}