cmake_minimum_required(VERSION 3.10)

project(PaletteConverter CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.56 REQUIRED COMPONENTS program_options)

file(GLOB CPP_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(palette_converter ${CPP_SOURCES})

target_compile_options(palette_converter PRIVATE -Wall -Wextra -O3)
target_link_libraries(palette_converter Boost::program_options)
