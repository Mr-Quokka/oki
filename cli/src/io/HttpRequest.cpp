#include "HttpRequest.h"

#include <cstring>
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

    static std::size_t writeContentTypeHeader(char *in, std::size_t size, std::size_t nmemb, std::string *out) {
        std::size_t totalSize = size * nmemb;

        std::size_t contentTypeLength = sizeof("Content-Type") - 1; // Équivalent à strlen mais à la compilation
        if (contentTypeLength >= (totalSize - 4)) {
            return totalSize; // Trop court
        }

        if (strncasecmp(in, "Content-Type", contentTypeLength) == 0) { // Cherche si le début de l'en-tête est 'Content-Type'
            const char *value = in + contentTypeLength + 2;            // Se positionne au début de la valeur, après ':'
            std::size_t length = totalSize - contentTypeLength - 4;    // Il reste à la lire la valeur, sans ': ' au début, ni \r et \n à la fin
            out->append(value, length);
        }
        return totalSize;
    }

    HttpRequest::HttpRequest(std::string_view url) : curl{curl_easy_init()}, headers{nullptr}, url{url} {
        curl_easy_setopt(curl, CURLOPT_URL, this->url.c_str());
    }

    HttpRequest::HttpRequest(const HttpRequest &request) : curl{curl_easy_init()}, headers{nullptr}, url{request.url} {
        auto *currentHeader = static_cast<struct curl_slist *>(request.headers);
        while (currentHeader) {
            addHeader(currentHeader->data);
            currentHeader = currentHeader->next;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    }

    HttpRequest &HttpRequest::operator=(HttpRequest other) {
        std::swap(curl, other.curl);
        std::swap(headers, other.headers);
        std::swap(url, other.url);
        return *this;
    }

    void HttpRequest::addHeader(const std::string &header) {
        addHeader(header.c_str());
    }

    void HttpRequest::addHeader(const char *header) {
        // Ajoute un nouveau maillon à la liste chaînée, la chaîne de caractères 'headers' étant dupliquée.
        headers = curl_slist_append(static_cast<struct curl_slist *>(headers), header);
    }

    HttpResponse HttpRequest::get() {
        std::string buffer;
        std::string contentType;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writeContentTypeHeader);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &contentType);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            throw RequestException{static_cast<int>(res)};
        }
        int httpStatus = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStatus);
        return HttpResponse{httpStatus, std::move(contentType), std::move(buffer)};
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

    const std::string &HttpRequest::getUrl() const {
        return url;
    }

    HttpRequest::~HttpRequest() {
        curl_slist_free_all(static_cast<struct curl_slist *>(headers));
        curl_easy_cleanup(curl);
    }

    HttpResponse::HttpResponse(int statusCode, std::string contentType, std::string content)
        : statusCode{statusCode}, contentType{std::move(contentType)}, content{std::move(content)} {}

    int HttpResponse::getStatusCode() const {
        return statusCode;
    }

    const std::string &HttpResponse::getContentType() const {
        return contentType;
    }

    const std::string &HttpResponse::getContent() const {
        return content;
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
