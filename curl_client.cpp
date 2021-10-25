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
    if(curl_)
    {
        curl_easy_cleanup(curl_);
    }
    curl_ = curl_easy_init();

    responseBody_.clear();
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
    //chunk = curl_slist_append(chunk, ";");

    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, chunk);
}

CurlClient_t::HttpResponse_t CurlClient_t::sendMessage(
        const Method& method,
        const std::string& uri,
        const std::string& content,
        const std::string& contentType,
        const std::string& user,
        const std::string& password,
        const bool useSSL
) {
    HttpResponse_t result = HttpResponse_t();

    init();


    using headerPairs = std::vector<std::pair<std::string, std::string>>;
    headerPairs requestHeader;



    //set url
    curl_easy_setopt(curl_, CURLOPT_URL, uri.c_str());

    //set method
    switch (method) {
        case HTTP_GET:
            curl_easy_setopt(curl_, CURLOPT_HTTPGET, 1L);
            break;
        case HTTP_POST:
            curl_easy_setopt(curl_, CURLOPT_HTTPPOST, 1L);
            requestHeader.push_back(std::make_pair("Content-Type", contentType));

            //set request body
            curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, content.c_str());
            break;
        default:
            //TODO error
            curl_easy_setopt(curl_, CURLOPT_HTTPGET, 1L);
            break;
    }





    //verify certificate
    if (useSSL){
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, 1L);
    }else{
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, 0L);
    }

    //follow redirect
    curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1L);

    //set request header
    setHeaders(requestHeader);



    //set port

    //set auth

    //TODO remove
    //curl_easy_setopt (curl_, CURLOPT_VERBOSE, 1L);

    //prepare callback
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &responseBody_);

    curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, WriteCallback);
    curl_easy_setopt(curl_, CURLOPT_HEADERDATA, &responseHeader_);

    //send message
    CURLcode res = curl_easy_perform(curl_);

    if(res == CURLE_OK){//TODO
        result.body = responseBody_;
        result.header = responseHeader_;
        result.success = true;

        int response_code;
        curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &response_code);
        result.httpStatus = static_cast<Status>( response_code);
    }

    //cleanup
    curl_easy_cleanup(curl_);

    return  result;
}

