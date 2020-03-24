# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.
cmake_minimum_required(VERSION 3.4.1)
project(other)

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)
add_library(first-lib
        STATIC
        first-invoke.cpp
        )
add_library(second-lib
        STATIC
        second-invoke.cpp
        )
#target_link_libraries(
#        first-lib
#        ${log-lib}
#)
#target_link_libraries(
#        second-lib
#        ${log-lib}
#)
#target_include_directories(first-lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
#
#target_include_directories(second-lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
#message("hello")