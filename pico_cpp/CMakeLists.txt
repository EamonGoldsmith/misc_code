cmake_minimum_required(VERSION 3.13)

# Include the Pico SDK initialization script
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(notary)

# Initialize the Pico SDK
pico_sdk_init()

# Add your executable and source files
add_executable(notary
    main.cpp
)

# Enable USB stdio and disable UART stdio
pico_enable_stdio_usb(notary 1)
pico_enable_stdio_uart(notary 0)

# Link the Pico SDK to your project
target_link_libraries(notary pico_stdlib hardware_adc)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(notary)
