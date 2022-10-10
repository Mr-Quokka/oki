#pragma once

#include <string>

class HttpRequest {
private:
    void *curl;
    std::string url;
public:
    explicit HttpRequest(std::string_view url);
    std::string get();
    ~HttpRequest();
};

class RequestException : public std::exception {
private:
    int code;
public:
    explicit RequestException(int code);
    const char *what() const noexcept override;
};
