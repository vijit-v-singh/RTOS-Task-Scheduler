# This file is required to find the Pico SDK.
# It should be in the root of your project.

# If the PICO_SDK_PATH is not defined, then default to searching
# three levels up from this project folder.
if (NOT PICO_SDK_PATH)
    set(PICO_SDK_PATH "../../pico-sdk")
endif ()

# Sanity check PICO_SDK_PATH
if (NOT EXISTS "${PICO_SDK_PATH}/pico_sdk_init.cmake")
    message(FATAL_ERROR "PICO_SDK_PATH is not pointing to a valid SDK directory. Did you run 'git submodule update --init'?")
endif ()

# This is the file that needs to be included
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)