cmake_minimum_required(VERSION 3.12)

project(R-Type LANGUAGES CXX)

option(BUILD_DOC "Build documentation" OFF)

set(CMAKE_CXX_STANDARD 20)
if(MSVC)
    # MSVC-specific warning flags
#    add_compile_options(/W4)  # Highest warning level for MSVC
else()
    # GCC/Clang warning flags
    add_compile_options(-Wall -Wextra)
endif()

enable_testing()

if (BUILD_DOC)
    add_subdirectory(dev_docs)
endif()

if(APPLE)
    find_library(COREAUDIO_LIBRARY CoreAudio)
    find_library(AUDIOUNIT_LIBRARY AudioUnit)
endif()

add_subdirectory(common)
add_subdirectory(network)
add_subdirectory(client)
add_subdirectory(master_server)
add_subdirectory(game_server)
