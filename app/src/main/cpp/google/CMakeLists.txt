#cmake 在windows 里面编译的目录
# cd D:\code\gitlab\NDK_Test\app\.cxx\cmake\debug\arm64-v8a
#cmd.exe /C "cd . && D:\env\android_sdk_for_compile\ndk\20.0.5594570\toolchains\llvm\prebuilt\windows-x86_64\bin\clang++.exe -v --target=aarch64-none-linux-android21 --gcc-toolchain=D:/env/android_sdk_for_compile/ndk/20.0.5594570/toolchains/llvm/prebuilt/windows-x86_64 --sysroot=D:/env/android_sdk_for_compile/ndk/20.0.5594570/toolchains/llvm/prebuilt/windows-x86_64/sysroot -fPIC -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -fno-addrsig -Wa,--noexecstack -Wformat -Werror=format-security  -std=c++11 -O0 -fno-limit-debug-info  -Wl,--exclude-libs,libgcc.a -Wl,--exclude-libs,libatomic.a -static-libstdc++ -Wl,--build-id -Wl,--warn-shared-textrel -Wl,--fatal-warnings -Wl,--no-undefined -Qunused-arguments -Wl,-z,noexecstack -shared -Wl,-soname,libnative-lib.so -o D:\code\gitlab\NDK_Test\app\build\intermediates\cmake\debug\obj\arm64-v8a\libnative-lib.so CMakeFiles/native-lib.dir/native-lib.cpp.o CMakeFiles/native-lib.dir/rename-thread.cpp.o CMakeFiles/native-lib.dir/memory-alloc.cpp.o CMakeFiles/native-lib.dir/cpptest.cpp.o  -llog other/libfirst-lib.a other/libsecond-lib.a -latomic -lm && cd ."
cmake_minimum_required(VERSION 3.4.1)
project(google_speech_jni)
set(google_speech_micro_jni google_speech_micro_jni)
add_library( # Sets the name of the library.
        ${PROJECT_NAME}

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        com_google_speech_recognizer_AbstractRecognizer.cpp
        )
add_library( # Sets the name of the library.
        ${google_speech_micro_jni}

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        com_google_speech_micro_GoogleHotwordData.cpp
        )

target_link_libraries( # Specifies the target library.
        ${PROJECT_NAME}
        log
        dl
        )
target_link_libraries( # Specifies the target library.
        ${google_speech_micro_jni}
        log
        dl
        )