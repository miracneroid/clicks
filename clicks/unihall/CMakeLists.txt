cmake_minimum_required(VERSION 3.21)
if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(example_unihall LANGUAGES MikroC)
else()
    project(example_unihall LANGUAGES C ASM)
endif()

if (NOT PROJECT_TYPE)
    set(PROJECT_TYPE "mikrosdk" CACHE STRING "" FORCE)
endif()

add_executable(example_unihall
        example/main.c

)


############################ example_unihall GENERATED CODE START ###########################
find_package(MikroC.Core REQUIRED)
target_link_libraries(example_unihall PUBLIC MikroC.Core)
find_package(MikroSDK.Board REQUIRED)
target_link_libraries(example_unihall PUBLIC MikroSDK.Board)
find_package(MikroSDK.Log REQUIRED)
target_link_libraries(example_unihall PUBLIC MikroSDK.Log)
add_subdirectory(lib_unihall)
target_link_libraries(example_unihall PUBLIC Click.UniHall)
############################ example_unihall GENERATED CODE END ###########################
