cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(lib_gps3 LANGUAGES MikroC)
else()
    project(lib_gps3 LANGUAGES C ASM)
endif()

if (NOT PROJECT_TYPE)
    set(PROJECT_TYPE "mikrosdk" CACHE STRING "" FORCE)
endif()

add_library(lib_gps3 STATIC
        src/gps3.c
        include/gps3.h
)
add_library(Click.Gps3  ALIAS lib_gps3)



target_include_directories(lib_gps3 PUBLIC
        include
)

find_package(MikroC.Core REQUIRED)
target_link_libraries(lib_gps3 PUBLIC MikroC.Core)
find_package(MikroSDK.Driver REQUIRED)
target_link_libraries(lib_gps3 PUBLIC MikroSDK.Driver)
find_package(MikroSDK.GenericPointer REQUIRED)
target_link_libraries(lib_gps3 PUBLIC MikroSDK.GenericPointer)
