cmake_minimum_required(VERSION 3.13)

include(pico-sdk/pico_sdk_init.cmake)
include(pico-sdk/tools/CMakeLists.txt)

# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 17)

project(
    quark
    VERSION 0.0.1
    LANGUAGES C CXX ASM
    DESCRIPTION "RP2040 Zero Waveshare")

pico_sdk_init()

set( QUARK_INCLUDE_DIR include/quark )
set( QUARK_SOURCE_DIR src )

add_executable(
    ${PROJECT_NAME}
    main.cpp
    # ${QUARK_INCLUDE_DIR}/app/application.h
    # ${QUARK_INCLUDE_DIR}/drivers/ssd1306.h
    # ${QUARK_INCLUDE_DIR}/drivers/ws2812.hpp
    # ${QUARK_INCLUDE_DIR}/font/font.h

    # ${QUARK_SOURCE_DIR}/app/application.cpp
    # ${QUARK_SOURCE_DIR}/drivers/ssd1306.cpp
    # ${QUARK_SOURCE_DIR}/drivers/ws2812.cpp
    # ${QUARK_SOURCE_DIR}/font/font.cpp
)

# pico_generate_pio_header(
#     ${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio
#     UTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

# enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(
    ${PROJECT_NAME} 
    PUBLIC
        pico_stdlib
        # pico_binary_info
        # hardware_pio
        # hardware_i2c
)

# Specifies include directories to use when compiling a given target
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(
    ${PROJECT_NAME}
)
