//
// Created by soellne2 on 22.10.2021.
//

#include "CurlClient.h"


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

CurlClient_t::CurlClient_t() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    init();
}

void CurlClient_t::init(){
    curl_ = curl_easy_init();

    responseBody_.clear();
    responseHeader_.clear();
}

CurlClient_t::~CurlClient_t() {
    curl_global_cleanup();
}

void CurlClient_t::setHeaders(std::vector<std::pair<std::string, std::string>>& values){
    struct curl_slist *chunk = NULL;

    for (auto &pair : values)
    {
        chunk = curl_slist_append(chunk, (pair.first + ": " + pair.second).c_str());

    }

    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, chunk);
}

CurlClient_t::HttpResponse_t CurlClient_t::sendMessage(
        const Method& method,
        const std::string& uri,
        const std::string& content,
        const std::string& contentType,
        const size_t& port,
        const bool useSSL
)
{
    HttpResponse_t response = HttpResponse_t();

    init();

    using headerPairs = std::vector<std::pair<std::string, std::string>>;
    headerPairs requestHeader;

    //set url
    curl_easy_setopt(curl_, CURLOPT_URL, uri.c_str());
    curl_easy_setopt(curl_, CURLOPT_PORT, port);

    //set method
    switch (method) {
        case HTTP_GET:
            curl_easy_setopt(curl_, CURLOPT_HTTPGET, 1L);
            break;
        case HTTP_POST:
            curl_easy_setopt(curl_, CURLOPT_HTTPPOST, 1L);
            requestHeader.push_back(std::make_pair("Content-Type", contentType));

            //set request header
            setHeaders(requestHeader);

            //set request body
            curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, content.c_str());
            break;
        default:
            //TODO throw exception error
            //TODO additional http methodes
            curl_easy_setopt(curl_, CURLOPT_HTTPGET, 1L);
    }

    //verify certificate
    if (useSSL)
    {
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, 1L);
    }
    else
    {
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, 0L);
    }

    //follow redirect
    //curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1L);

    //TODO: set auth

    //prepare callback
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &responseBody_);

    curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, WriteCallback);
    curl_easy_setopt(curl_, CURLOPT_HEADERDATA, &responseHeader_);

    //send message
    CURLcode res = curl_easy_perform(curl_);

    if(res == CURLE_OK){//TODO
        response.body = responseBody_;
        response.header = responseHeader_;
        response.success = true;

        int response_code;
        curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &response_code );
        response.httpStatus = static_cast<Status>( response_code );
    }
    else
    {
        // TODO: HELP
    }

    //cleanup
    curl_easy_cleanup(curl_);

    return  response;
}

