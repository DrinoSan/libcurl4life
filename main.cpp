#include <iostream>
#include <curl/curl.h>
#include "CurlClient.h"

//using CurlClient_t;

int main() {


    CurlClient_t client = CurlClient_t();
    CurlClient_t::HttpResponse_t sand = client.sendMessage(CurlClient_t::HTTP_POST, "https://sardino.free.beeceptor.com", "{\"mit contentTypeAlsVar\": \"FOOO\"}","application/json", 443L);
    CurlClient_t::HttpResponse_t sand2 = client.sendMessage(CurlClient_t::HTTP_GET, "https://gorest.co.in/public/v1/users/35", "{\"mit contentTypeAlsVar\": \"FOOO\"}","application/json", 443L);
    //CurlClient_t::HttpResponse_t sand= client.sendMessage(CurlClient_t::HTTP_GET, "www.google.com", "","","","", false);

    std::cout << sand.body << std::endl;
    std::cout << std::endl;
    std::cout << sand2.body << std::endl;

    return 0;
}

//
//size_t CurlWrite_CallbackFunc_StdString(void *contents, size_t size, size_t nmemb, std::string *s)
//{
//    size_t newLength = size*nmemb;
//    try
//    {
//        s->append((char*)contents, newLength);
//    }
//    catch(std::bad_alloc &e)
//    {
//        //handle memory problem
//        return 0;
//    }
//    return newLength;
//}
//
//int main()
//{
//    CURL *curl;
//    CURLcode res;
//
//    curl_global_init(CURL_GLOBAL_DEFAULT);
//
//    curl = curl_easy_init();
//    std::string s;
//    if(curl)
//    {
//
//        //curl_easy_setopt(curl, CURLOPT_URL, "http://demo2996169.mockable.io/");
//        //curl_easy_setopt(curl, CURLOPT_URL, "https://10.212.11.40/service/stations/transfer_stations");
//        curl_easy_setopt(curl, CURLOPT_URL, "gorest.co.in:443/public/v1/users/3812");
//
//        //curl_easy_setopt(curl, CURLOPT_USERPWD, "admin:admin");
//
//        //follow redirect
//        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//
//        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //only for https
//        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); //only for https
//
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
//
//        curl_easy_setopt (curl, CURLOPT_VERBOSE, 1L); //remove this to disable verbose output
//        curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
//
//
//        /* Perform the request, res will get the return code */
//        res = curl_easy_perform(curl);
//        /* Check for errors */
//        if(res != CURLE_OK)
//        {
//            fprintf(stderr, "curl_easy_perform() failed: %s\n",
//                    curl_easy_strerror(res));
//        }
//
//        /* always cleanup */
//        curl_easy_cleanup(curl);
//    }
//
//    std::cout<< "----------------------------Response Body Start \r\n" <<s<< "\r\n  --------------------------Response Body end"<<std::endl;
//
//    std::cout<< "Program finished!" << std::endl;
//}