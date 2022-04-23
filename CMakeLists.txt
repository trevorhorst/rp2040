cmake_minimum_required(VERSION 3.13)

include(pico-sdk/pico_sdk_init.cmake)
include(pico-sdk/tools/CMakeLists.txt)

# Creates a portable way to implement the __FILENAME__ macro
# https://stackoverflow.com/questions/1706346/file-macro-manipulation-handling-at-compile-time
function(define_file_basename_for_sources targetname)
    get_target_property(source_files "${targetname}" SOURCES)
    foreach(sourcefile ${source_files})
        # Add the FILE_BASENAME=filename compile definition to the list.
        get_filename_component(basename "${sourcefile}" NAME)
        # Set the updated compile definitions on the source file.
        set_property(
            SOURCE "${sourcefile}" APPEND
            PROPERTY COMPILE_DEFINITIONS "__FILENAME__=\"${basename}\"")
    endforeach()
endfunction()

project(
    rp2040
    VERSION 0.0.1
    LANGUAGES C CXX ASM
    DESCRIPTION "RP2040 Zero Waveshare")

pico_sdk_init()

set( RP2040_INCLUDE_DIR include/${PROJECT_NAME} )
set( RP2040_SOURCE_DIR src )

add_executable(
    ${PROJECT_NAME}
    main.cpp
    ${RP2040_INCLUDE_DIR}/application.h
    ${RP2040_INCLUDE_DIR}/console.h
    ${RP2040_INCLUDE_DIR}/drivers/ssd1306.h
    ${RP2040_INCLUDE_DIR}/drivers/ws2812.h
    ${RP2040_INCLUDE_DIR}/font/font.h
    ${RP2040_INCLUDE_DIR}/logger.h

    ${RP2040_SOURCE_DIR}/application.cpp
    ${RP2040_SOURCE_DIR}/console.cpp
    ${RP2040_SOURCE_DIR}/drivers/ssd1306.cpp
    ${RP2040_SOURCE_DIR}/drivers/ws2812.cpp
    ${RP2040_SOURCE_DIR}/font/font.cpp
    ${RP2040_SOURCE_DIR}/logger.cpp
)

pico_generate_pio_header(
    ${PROJECT_NAME} 
        ${CMAKE_CURRENT_LIST_DIR}/pio/ws2812.pio
    UTPUT_DIR 
        ${CMAKE_CURRENT_LIST_DIR}/generated
)

# enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(
    ${PROJECT_NAME} 
    PUBLIC
        pico_stdlib
        pico_multicore
        # pico_binary_info
        hardware_pio
        hardware_i2c
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

# Create the __FILENAME__ macro for use with the logger
define_file_basename_for_sources(${PROJECT_NAME})

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(
    ${PROJECT_NAME}
)
