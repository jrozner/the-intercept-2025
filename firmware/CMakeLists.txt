cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(firmware_project C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(firmware
  main.c
  menu.c
  mining.c
  config.c
  tiny_md5.c
)

pico_enable_stdio_usb(firmware 1)

target_link_libraries(firmware pico_stdlib)
