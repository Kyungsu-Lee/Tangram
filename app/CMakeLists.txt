# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

set(pathOpenCv /Users/lmasi/Documents/AndroidProject/OpenCV-android-sdk)
set(pathProject /Users/lmasi/Documents/AndroidProject/Tangram)

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

include_directories(${pathOpenCv}/sdk/native/jni/include)

add_library( lib_opencv SHARED IMPORTED )
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${pathProject}/app/src/main/JniLibs/${ANDROID_ABI}/libopencv_java3.so)

#와일드 문자를 이용하여 여러개의 파일을 불러올 수 도 있음
# *.cpp 또는 *.h
file(GLOB Library_SOURCES  src/main/cpp/Tangram2.cpp)
file(GLOB Library_HEADER   src/main/cpp/Tangram2.h)

#공유 라이브러리 생성
add_library(imported-lib SHARED ${Library_SOURCES} ${Library_HEADERS})
set_target_properties(imported-lib PROPERTIES LINKER_LANGUAGE CXX)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.
             src/main/cpp/native-lib.cpp )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because system libraries are included in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

#add_library( lib_opencv SHARED IMPORTED )
#set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${pathProject}/app/src/main/JniLibs/${ANDROID_ABI}/libopencv_java3.so)


find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( imported-lib
                        lib_opencv)

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                        imported-lib
                        lib_opencv)
