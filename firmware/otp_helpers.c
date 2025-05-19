#include <stdio.h>
#include "pico/bootrom.h"
#include "hardware/regs/otp_data.h"
#include "hardware/structs/otp.h"

#include "otp_helpers.h"

// this was all stolen from https://github.com/raspberrypi/rp2350_hacking_challenge/blob/main/main.c

bool is_locked() {
  volatile uint32_t *otp_page48_lock1_ptr = ((uint32_t *)(OTP_DATA_BASE + (0xfe0*2)));

  if (otp_page48_lock1_ptr[0] != 0x3C3C0000) {
    return false;
  }

  if (otp_page48_lock1_ptr[1] != 0x3C) {
    return false;
  }

  return true;
}

void lock_otp_secret() {
  otp_cmd_t cmd;
  cmd.flags = OTP_DATA_PAGE48_LOCK1_ROW | OTP_CMD_ECC_BITS | OTP_CMD_WRITE_BITS;
  // 3 redundant copies
  uint32_t value = 0x3c3c3c;
  uint32_t ret = rom_func_otp_access((uint8_t *)&value, sizeof(value), cmd);
  if (ret) {
    printf("\tLocking failed with error: %d\n", ret);
    while(1) {

    }
  }
}

void lock_sw_lock_48() {
  // Lock the OTP memory so it's not readable afterwards
  // - [3:2] - Non-Secure lock status
  // - [1:0] - Secure lock status
  // - 0 = read_write 1 = read_only 3 = inaccessible
  otp_hw->sw_lock[48] = 0b1111;
}
