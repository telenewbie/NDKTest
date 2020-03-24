cmake_minimum_required(VERSION 3.4.1)
project(charset)
add_library(${PROJECT_NAME} SHARED charset_util.cpp tconv_g2u.cpp tconv_comm.cpp charsetUtils.cpp)
#target_compile_options(${PROJECT_NAME} PUBLIC -funwind-tables -g)
#        -Wno-psabi
#        -Os -fvisibility=hidden)
target_link_libraries(${PROJECT_NAME} -llog)