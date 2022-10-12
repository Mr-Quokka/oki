#include "HttpRequest.h"

#include <curl/curl.h>

namespace oki{
    static std::size_t writeCallback(char *in, std::size_t size, std::size_t nmemb, std::string *out) {
        std::size_t totalSize = size * nmemb;
        if (totalSize) {
            out->append(in, totalSize);
            return totalSize;
        }
        return 0;
    }

    HttpRequest::HttpRequest(std::string_view url) : curl{curl_easy_init()}, url{url} {
        curl_easy_setopt(curl, CURLOPT_URL, this->url.c_str());
    }

    std::string HttpRequest::get() {
        std::string buffer;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            throw RequestException{static_cast<int>(res)};
        }
        return buffer;
    }

    HttpRequest::~HttpRequest() {
        curl_easy_cleanup(curl);
    }

    RequestException::RequestException(int code) : code{code} {}

    const char *RequestException::what() const noexcept {
        return curl_easy_strerror(static_cast<CURLcode>(code));
    }

    APIExeption::APIExeption(std::string_view msg) : msg{msg} {}

    const char *APIExeption::what() const noexcept {
        return this->msg.c_str();
    }
}