cmake_minimum_required(VERSION 3.4.1)
project(dns)
set (CMAKE_CXX_STANDARD 11)
add_library(${PROJECT_NAME} SHARED txz_cache_dns.cpp txz_dns.cpp android_bridge.cpp Int2Ip.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ../include)
#target_compile_options(${PROJECT_NAME} PUBLIC -funwind-tables -g)
#        -Wno-psabi
#        -Os -fvisibility=hidden)
target_link_libraries(${PROJECT_NAME} -llog)