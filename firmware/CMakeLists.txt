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
  otp_helpers.c
)

pico_enable_stdio_usb(firmware 1)

target_link_libraries(firmware pico_stdlib)

# Signing and hashing
pico_set_binary_type(firmware no_flash)
pico_sign_binary(firmware ${CMAKE_CURRENT_SOURCE_DIR}/keys/private.pem)
pico_hash_binary(firmware)
pico_package_uf2_output(firmware 0x10000000)
pico_set_otp_key_output_file(firmware ${CMAKE_CURRENT_LIST_DIR}/otp.json)

pico_add_extra_outputs(firmware)
