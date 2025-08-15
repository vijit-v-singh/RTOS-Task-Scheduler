# CMake build file for the Pico FreeRTOS Scheduler project

# Minimum CMake version
cmake_minimum_required(VERSION 3.13)

# Import pico_sdk.cmake before project()
include(pico_sdk_import.cmake)

# Define the project
project(pico_freertos_scheduler C CXX ASM)

# Set C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

# --- FreeRTOS Configuration ---
# Add the FreeRTOS Kernel submodule directory
add_subdirectory(lib/FreeRTOS-Kernel)

# Add the project executable
add_executable(${CMAKE_PROJECT_NAME}
    src/main.c
)

# --- Link Libraries ---
# Link pico_stdlib and hardware_adc for sensor reading
target_link_libraries(${CMAKE_PROJECT_NAME}
    pico_stdlib
    hardware_adc
    FreeRTOS-Kernel
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 1)
pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 0)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})

# --- Final Message ---
message(STATUS "Project Name: ${CMAKE_PROJECT_NAME}")
message(STATUS "Binary: ${CMAKE_PROJECT_NAME}.uf2")