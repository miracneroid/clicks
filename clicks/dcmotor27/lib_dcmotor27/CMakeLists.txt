cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(lib_dcmotor27 LANGUAGES MikroC)
else()
    project(lib_dcmotor27 LANGUAGES C ASM)
endif()

if (NOT PROJECT_TYPE)
    set(PROJECT_TYPE "mikrosdk" CACHE STRING "" FORCE)
endif()

add_library(lib_dcmotor27 STATIC
        src/dcmotor27.c
        include/dcmotor27.h
)
add_library(Click.DCMotor27  ALIAS lib_dcmotor27)



target_include_directories(lib_dcmotor27 PUBLIC 
        include
)

find_package(MikroC.Core REQUIRED)
target_link_libraries(lib_dcmotor27 PUBLIC MikroC.Core)
find_package(MikroSDK.Driver REQUIRED)
target_link_libraries(lib_dcmotor27 PUBLIC MikroSDK.Driver)

