cmake_minimum_required(VERSION 3.26)
project(punchcutter C)

set(CMAKE_C_STANDARD 11)

find_package(cmark REQUIRED)
set (EXTERNAL_INCLUDES dmon/dmon.h)

add_executable(punchcutter main.c
        utils/array.c
        utils/string.c
        utils/file.c
        utils/file.unix.c
        utils/file.windows.c
        utils/utils.h
        command/generator.c
        command/node.c
        command/scanner.c
        command/watch.c
        command/command.h
)

if (APPLE)
    target_link_libraries(punchcutter PRIVATE
        cmark::cmark
        "-framework CoreFoundation"
        "-framework CoreServices"
    )
else ()
    target_link_libraries(punchcutter PRIVATE cmark::cmark)
endif ()
