cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(firmware_project C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# Encrypted Bootloader
add_executable(enc_bootloader
        enc_bootloader.c
        aes.S
        )

pico_enable_stdio_uart(enc_bootloader 1)

# Add command to update otp.json if privateaes.bin changes
add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/otp.json
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_LIST_DIR}/update-key.cmake"
    DEPENDS ${CMAKE_CURRENT_LIST_DIR}/privateaes.bin)
# Copy that otp.json file to build directory
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/otp.json
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_LIST_DIR}/otp.json" "${CMAKE_CURRENT_BINARY_DIR}/otp.json"
    DEPENDS ${CMAKE_CURRENT_LIST_DIR}/otp.json)
add_custom_target(otp_json DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/otp.json)
add_dependencies(enc_bootloader otp_json)

# pull in common dependencies
target_link_libraries(enc_bootloader pico_stdlib pico_rand)

# use stack guards, as AES variables are written near the stack
target_compile_definitions(enc_bootloader PRIVATE PICO_USE_STACK_GUARDS=1)

# set as no_flash binary
pico_set_binary_type(enc_bootloader no_flash)

# Add a linker script to run no_flash binary from anywhere
function(add_linker_script target origin length)
    # Write script file to run later, to generate the linker script
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/make_linker_script.cmake"
        "# create linker script to run from elsewhere in SRAM\n"
        "file(READ \${PICO_LINKER_SCRIPT_PATH}/memmap_no_flash.ld LINKER_SCRIPT)\n"
        "string(REPLACE \"RAM(rwx) : ORIGIN =  0x20000000, LENGTH = 512k\" \"RAM(rwx) : ORIGIN =  \${origin}, LENGTH = \${length}\" LINKER_SCRIPT \"\${LINKER_SCRIPT}\")\n"
        "file(WRITE \${output_file} \"\${LINKER_SCRIPT}\")\n"
    )
    # Add command to run this whenever memmap_no_flash.ld changes
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${target}.ld
        COMMAND ${CMAKE_COMMAND}
            -DPICO_LINKER_SCRIPT_PATH:PATH=${PICO_LINKER_SCRIPT_PATH}
            -Dorigin="${origin}" -Dlength="${length}"
            -Doutput_file:FILEPATH=${CMAKE_CURRENT_BINARY_DIR}/${target}.ld
            -P "${CMAKE_CURRENT_BINARY_DIR}/make_linker_script.cmake"
        DEPENDS ${PICO_LINKER_SCRIPT_PATH}/memmap_no_flash.ld)
    add_custom_target(${target}_ld DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${target}.ld)
    add_dependencies(${target} ${target}_ld)
    pico_set_linker_script(${target} ${CMAKE_CURRENT_BINARY_DIR}/${target}.ld)
endfunction()

# create linker script to run from 0x20070000
add_linker_script(enc_bootloader "0x20070000" "64k")

# configure otp output
pico_set_otp_key_output_file(enc_bootloader ${CMAKE_CURRENT_BINARY_DIR}/otp.json)

# sign, hash, and clear SRAM
pico_sign_binary(enc_bootloader ${CMAKE_CURRENT_LIST_DIR}/keys/private.pem)
pico_hash_binary(enc_bootloader)
pico_load_map_clear_sram(enc_bootloader)

# add partition table
pico_embed_pt_in_binary(enc_bootloader ${CMAKE_CURRENT_LIST_DIR}/enc-pt.json)

# create absolute uf2, and package in flash
pico_set_uf2_family(enc_bootloader "absolute")
pico_package_uf2_output(enc_bootloader 0x10000000)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(enc_bootloader)

# firmware

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

add_linker_script(firmware "0x20000000" "448k")

pico_sign_binary(firmware ${CMAKE_CURRENT_SOURCE_DIR}/keys/private.pem)
pico_hash_binary(firmware)
pico_encrypt_binary(firmware ${CMAKE_CURRENT_LIST_DIR}/privateaes.bin)
pico_package_uf2_output(firmware 0x10000000)
#pico_set_otp_key_output_file(firmware ${CMAKE_CURRENT_LIST_DIR}/otp.json)

pico_add_extra_outputs(firmware)
