cmake_minimum_required(VERSION 3.4.1)
project(txzlog)
add_library(${PROJECT_NAME} SHARED JNILogManager.cpp ILogAdapter_jni.cpp)
#target_compile_options(${PROJECT_NAME} PUBLIC -funwind-tables -g)
#        -Wno-psabi
#        -Os -fvisibility=hidden)
target_link_libraries(${PROJECT_NAME} -llog)