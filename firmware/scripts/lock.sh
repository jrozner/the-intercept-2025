#!/bin/bash

set -e
set -v

# Disable debugging features
picotool otp set OTP_DATA_CRIT1.DEBUG_DISABLE 1 
# Disable other boot keys
picotool otp set OTP_DATA_BOOT_FLAGS1.KEY_INVALID 0xe
# Enable glitch detector
picotool otp set OTP_DATA_CRIT1.GLITCH_DETECTOR_ENABLE 1
# Highest sensitivity
picotool otp set OTP_DATA_CRIT1.GLITCH_DETECTOR_SENS 3
