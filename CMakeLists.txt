cmake_minimum_required(VERSION 3.20)
project(curlClient)

set(CMAKE_CXX_STANDARD 14)

set(Curlcpp_INCLUDE_DIR /opt/homebrew/Cellar/curlpp/0.8.1_1/include)
set(Curlcpp_LIBRARY_DIR /opt/homebrew/Cellar/curlpp/0.8.1_1/lib)
set(CURLCPP_ROOT /opt/homebrew/Cellar/boost/1.76.0)
find_package(Boost 1.76.0)
include_directories(${Curlcpp_INCLUDE_DIR})




add_executable(curlClient main.cpp curl.h CurlClient.h curl_client.cpp)
target_link_libraries(curlClient curl)