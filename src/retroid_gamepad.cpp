#include <bitset>
#include <cstring>

#include "udp_gamepad/retroid_gamepad.h"

namespace udp_gamepad {
/**
 * @brief Constructor for the RetroidGamepad class.
 *
 * Initializes the Retroid gamepad with the specified UDP port for data reception.
 *
 * @param port The UDP port to use for receiving gamepad data.
 */
RetroidGamepad::RetroidGamepad(int port) : Gamepad(port) {
  button_status_.clear();
  memset(&keys_, 0, sizeof(keys_));
}

/**
 * @brief Updates the RETROID gamepad data buffer and keys.
 *
 * @param buffer The data buffer to be updated.
 * @param keys The RETROID gamepad keys to be updated.
 * @return True if the data is valid and updated, false otherwise.
 */
bool RetroidGamepad::UpdateData(std::vector<uint8_t> &buffer, RetroidKeys &keys) {
  RetroidGamepadData data{};
  timespec timestamp{};
  memcpy(&data, buffer.data(), buffer.size() * sizeof(uint8_t));
  // Perform data validity check in the child class
  if (DataIsValid(data)) {
    std::bitset<kChannlSize> value_bit(0);
    int16_t ch[kChannlSize];
    memcpy(ch, data.data, sizeof(ch));
    for (int i = 0; i < kChannlSize; i++) {
      value_bit[i] = ch[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &timestamp);
    keys.time_stamp = static_cast<double>(timestamp.tv_sec - start_time_.tv_sec) * 1e3
        + static_cast<double>(timestamp.tv_nsec - start_time_.tv_nsec) / 1e6;
    keys.value = value_bit.to_ulong();

    keys.left = static_cast<uint8_t>(data.left_axis_x == -kJoystickRange
                                       ? KeyStatus::kPressed
                                       : KeyStatus::kReleased);
    keys.right = static_cast<uint8_t>(data.left_axis_x == kJoystickRange
                                        ? KeyStatus::kPressed
                                        : KeyStatus::kReleased);
    keys.up = static_cast<uint8_t>(data.left_axis_y == kJoystickRange
                                     ? KeyStatus::kPressed
                                     : KeyStatus::kReleased);
    keys.down = static_cast<uint8_t>(data.left_axis_y == -kJoystickRange
                                       ? KeyStatus::kPressed
                                       : KeyStatus::kReleased);

    keys.left_axis_x = data.left_axis_x / static_cast<float>(kJoystickRange);
    keys.left_axis_y = data.left_axis_y / static_cast<float>(kJoystickRange);
    keys.right_axis_x = data.right_axis_x / static_cast<float>(kJoystickRange);
    keys.right_axis_y = data.right_axis_y / static_cast<float>(kJoystickRange);

    return true;
  }
  return false;
}

/**
 * @brief Checks if the received data from the RETROID gamepad is valid.
 *
 * @param data The gamepad data to be validated.
 * @return True if the data is valid, false otherwise.
 */
bool RetroidGamepad::DataIsValid(const RetroidGamepadData &data) {
  // 1. check STX
  if (data.stx[0] != kHeader[0] || data.stx[1] != kHeader[1]) {
    return false;
  }

  // 2. check device ID
  if (data.id != (uint8_t)GamepadType::kRetroid) {
    return false;
  }

  // 3. CRC16
  if (data.crc16 != CalculateCrc16(data.data, data.data_len)) {
    return false;
  }

  return true;
}
} // namespace udp_gamepad
