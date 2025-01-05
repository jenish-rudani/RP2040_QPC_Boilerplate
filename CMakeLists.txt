# Generated Cmake Pico project file
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================


# == QP/C Configuration ==============================================================

# Configure the QP/C++ Real-Time Embedded Framework
set(QP_RTEF "QPC")
set(QP_KERNEL "QK")
# Don't Activate QP/Spy Software Tracing
set(QSPY 0)
#add_compile_options(-DQP_CONFIG)
include(qp_import.cmake)
# ====================================================================================


# == Pico Configuration ================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(blink C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(blink
    # blink.c
    bsp.c
    bsp.h
    app.c
    main.c
    pico_app.h
    ${QP_SOURCES}
    )

target_compile_options( blink PUBLIC
    ### Gnu/Clang C Options
    $<$<COMPILE_LANG_AND_ID:C,GNU>:-fdiagnostics-color=always>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:-fcolor-diagnostics>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wall>
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wextra>
    )

target_include_directories(blink PRIVATE ${QP_INCLUDE})

# pull in common dependencies
target_link_libraries(blink 
                        pico_stdlib
                        hardware_spi
                        hardware_gpio
                        hardware_timer
                        cmsis_core
                        )

# Enable USB output, disable UART output
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(blink)

# add url via pico_set_program_url

# ====================================================================================
