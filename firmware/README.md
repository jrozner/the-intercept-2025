# The Intercept 2025 Firmware

## Tooling Setup

Instructions for setting up the rp2350 toolchain can be found [here](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf). The primary steps are installing an arm toolchain, pico-sdk, and picotool.

## Building

1. `cmake -DPICO_SDK_PATH=<path to pico-sdk>`
2. `make`
