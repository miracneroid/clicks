cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(lib_rtc14 LANGUAGES MikroC)
else()
    project(lib_rtc14 LANGUAGES C ASM)
endif()

if (NOT PROJECT_TYPE)
    set(PROJECT_TYPE "mikrosdk" CACHE STRING "" FORCE)
endif()

add_library(lib_rtc14 STATIC
        src/rtc14.c
        include/rtc14.h
)
add_library(Click.RTC14  ALIAS lib_rtc14)



target_include_directories(lib_rtc14 PUBLIC
        include
)

find_package(MikroC.Core REQUIRED)
target_link_libraries(lib_rtc14 PUBLIC MikroC.Core)
find_package(MikroSDK.Driver REQUIRED)
target_link_libraries(lib_rtc14 PUBLIC MikroSDK.Driver)
