#include "HttpRequest.h"

#include <curl/curl.h>

namespace oki {
    static std::size_t writeCallback(char *in, std::size_t size, std::size_t nmemb, std::string *out) {
        std::size_t totalSize = size * nmemb;
        if (totalSize) {
            out->append(in, totalSize);
            return totalSize;
        }
        return 0;
    }

    static std::size_t writeDataCallback(char *in, std::size_t size, std::size_t nmemb, FILE *out) {
        return fwrite(in, size, nmemb, out);
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

    void HttpRequest::download(const std::filesystem::path &path) {
        // Utilisation de l'API C FILE plutôt qu'ofstream pour pouvoir faire un simple fwrite() avec les arguments renseignés par CURL.
        FILE *file = fopen(path.c_str(), "wb");
        if (file == nullptr) {
            throw std::system_error(errno, std::generic_category(), path);
        }
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeDataCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        CURLcode res = curl_easy_perform(curl);
        fclose(file);
        if (res != CURLE_OK) {
            throw RequestException{static_cast<int>(res)};
        }
    }

    HttpRequest::~HttpRequest() {
        curl_easy_cleanup(curl);
    }

    RequestException::RequestException(int code) : code{code} {}

    const char *RequestException::what() const noexcept {
        return curl_easy_strerror(static_cast<CURLcode>(code));
    }

    APIException::APIException(std::string_view msg) : msg{msg} {}

    const char *APIException::what() const noexcept {
        return this->msg.c_str();
    }
}