cmake_minimum_required(VERSION 3.26)
project(punchcutter C)

set(CMAKE_C_STANDARD 11)

find_package(cmark REQUIRED)

add_executable(punchcutter main.c
    utils/array.c
    utils/string.c
    utils/file.c
    utils/file.unix.c
    utils/file.windows.c
    utils/utils.h
    command/generator.c
    command/generator.h
    command/node.c
    command/node.h
    command/scanner.c
    command/scanner.h
)

target_link_libraries(punchcutter PRIVATE cmark::cmark)
