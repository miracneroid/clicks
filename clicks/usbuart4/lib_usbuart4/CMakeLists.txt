cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(lib_usbuart4 LANGUAGES MikroC)
else()
    project(lib_usbuart4 LANGUAGES C ASM)
endif()

if (NOT PROJECT_TYPE)
    set(PROJECT_TYPE "mikrosdk" CACHE STRING "" FORCE)
endif()

add_library(lib_usbuart4 STATIC
        src/usbuart4.c
        include/usbuart4.h
)
add_library(Click.USBUART4  ALIAS lib_usbuart4)



target_include_directories(lib_usbuart4 PUBLIC
        include
)

find_package(MikroC.Core REQUIRED)
target_link_libraries(lib_usbuart4 PUBLIC MikroC.Core)
find_package(MikroSDK.Driver REQUIRED)
target_link_libraries(lib_usbuart4 PUBLIC MikroSDK.Driver)
