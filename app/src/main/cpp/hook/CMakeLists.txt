# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.
cmake_minimum_required(VERSION 3.4.1)
project(myhook)
include_directories(include)
set(myhook_mem ${PROJECT_NAME}_mem)
message(${myhook_mem})

add_library(${myhook_mem} SHARED get_functions.cpp)
target_link_libraries(${myhook_mem} #[[-Wl,-Bsymbolic]] -llog -lz)


add_library(${PROJECT_NAME} SHARED replace_functions.cpp hookMem.cpp)

target_link_libraries(${PROJECT_NAME}
        ${myhook_mem}
        #[[-Wl,-Bsymbolic]]
        -llog -lz

        )

#target_link_options(${PROJECT_NAME} PUBLIC -Wl,-Bsymbolic)
#target_include_directories(${PROJECT_NAME} public include)
#target_include_directories(${myhook_mem} public include)