cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(example_touchclamp LANGUAGES MikroC)
else()
    project(example_touchclamp LANGUAGES C ASM)
endif()

if (NOT PROJECT_TYPE)
    set(PROJECT_TYPE "mikrosdk" CACHE STRING "" FORCE)
endif()

add_executable(example_touchclamp
        main.c

)


############################ example_touchclamp GENERATED CODE START ###########################
find_package(MikroC.Core REQUIRED)
target_link_libraries(example_touchclamp PUBLIC MikroC.Core)
find_package(MikroSDK.Board REQUIRED)
target_link_libraries(example_touchclamp PUBLIC MikroSDK.Board)
find_package(MikroSDK.Log REQUIRED)
target_link_libraries(example_touchclamp PUBLIC MikroSDK.Log)
add_subdirectory(lib_touchclamp)
target_link_libraries(example_touchclamp PUBLIC Click.TouchClamp)
############################ example_touchclamp GENERATED CODE END ###########################
