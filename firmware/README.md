# The Intercept 2025 Firmware

## Tooling Setup

Instructions for setting up the rp2350 toolchain can be found [here](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf). The primary steps are installing an arm toolchain, pico-sdk, and picotool.

We are building against commit 65a8907828ad87acd6a8e3363f175a64337b65cd in pico-sdk because we need the cmake fixes. If they release a new version we can pin it to that instead.

## Building

1. `mkdir build`
2. `cd build`
3. `cmake -DPICO_SDK_PATH=<path to pico-sdk> -DPICO_PLATFORM=rp2350 -DPICO_BOARD=pico2` ..
4. `make`
5. `picotool load firmware.elf`
