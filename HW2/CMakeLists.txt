cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)     ##include file on folder

include(pico_extras_import_optional.cmake)

project(HW2)   ### Change to folder name
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

add_executable(blink
        blink.c       # Name of c file
        )

# pull in common dependencies
target_link_libraries(blink pico_stdlib)  # First name of c file + pico_stdlib

# create map/bin/hex file etc.
pico_add_extra_outputs(blink)