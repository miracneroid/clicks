cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(lib_color17 LANGUAGES MikroC)
else()
    project(lib_color17 LANGUAGES C ASM)
endif()

if (NOT PROJECT_TYPE)
    set(PROJECT_TYPE "mikrosdk" CACHE STRING "" FORCE)
endif()

add_library(lib_color17 STATIC
        src/color17.c
        include/color17.h
)
add_library(Click.Color17  ALIAS lib_color17)



target_include_directories(lib_color17 PUBLIC 
        include
)

find_package(MikroC.Core REQUIRED)
target_link_libraries(lib_color17 PUBLIC MikroC.Core)
find_package(MikroSDK.Driver REQUIRED)
target_link_libraries(lib_color17 PUBLIC MikroSDK.Driver)

