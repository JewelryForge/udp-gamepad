#include "skydroid_gamepad.h"
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <bitset>


/**
 * @brief Constructor for the SkydroidGamepad class.
 *
 * Initializes the Skydroid controller with the specified UDP port for data reception.
 *
 * @param port The UDP port to use for receiving controller data.
 */
SkydroidGamepad::SkydroidGamepad(int port) : Gamepad(port) {

  std::string pressed  = fmt::format("\x1b[32m{:^20}\x1b[0m", "pressed");
  std::string released = fmt::format("\x1b[37m{:^20}\x1b[0m", "released");
  kButtonStatus.push_back(released);
  kButtonStatus.push_back(pressed);

}

/**
 * @brief Updates the Skydroid controller data buffer and keys.
 *
 * @param buffer The data buffer to be updated.
 * @param keys The Skydroid controller keys to be updated.
 * @return True if the data is valid and updated, false otherwise.
 */
bool SkydroidGamepad::UpdateData(std::vector<uint8_t>& buffer, SkydroidKeys& keys) {
  SkydroidGamepadData data;
  memcpy(&data, buffer.data(), buffer.size()*sizeof(uint8_t));
  // Perform data validity check in the child class
  if (DataIsValid(data)) {
    std::bitset<kSkydroidButtonSize> keys_value_bit(0);
    int16_t keys_ch[kSkydroidButtonSize];
    memcpy(keys_ch, data.buttons, sizeof(keys_ch));
    for(int i = 0; i < kSkydroidButtonSize; i++){
      keys_value_bit[i] = keys_ch[i];
    }

    keys.keys_value = keys_value_bit.to_ulong();
    keys.left_axis_x = data.left_axis_x/(float)kJoystickRange;
    keys.left_axis_y = data.left_axis_y/(float)kJoystickRange;
    keys.right_axis_x = data.right_axis_x/(float)kJoystickRange;
    keys.right_axis_y = data.right_axis_y/(float)kJoystickRange;

    keys.sw1 = data.switch_keys[0];
    keys.sw2 = data.switch_keys[1];
    keys.sw3 = data.switch_keys[2];
    keys.sw4 = data.switch_keys[3];

    return true;
  }
  return false;
}

/**
 * @brief Checks if the received data from the Skydroid controller is valid.
 *
 * @param data The controller data to be validated.
 * @return True if the data is valid, false otherwise.
 */
bool SkydroidGamepad::DataIsValid(const SkydroidGamepadData& data) {

  // 1. check STX   
  if (data.stx[0] != kHeader[0] || data.stx[1] != kHeader[1]) {
    return false;
  }

  // 2. check device ID
  if (data.id != (uint8_t)GamepadType::kSkydroid) {
    return false;
  }
  
  // 3. CRC16
  if(data.crc16 != CalculateCrc16(data.data, data.data_len)){
    return false;
  }

  return true;  
}

std::ostream& operator<<(std::ostream& o, SkydroidGamepad& is){
  
  SkydroidKeys keys = is.GetKeys();
  o << "\033c";
  std::string  s = fmt::format(
	"┌{0:─^{1}}┐\n"
	"│{2: ^{1}}│\n"
	"└{0:─^{1}}┘\n", "", 86 ,"Joystick Device: Skydorid(X30)");
  o<<s ;
  s = fmt::format(
    "┌{2:─^{1}}┐┌{3:─^{1}}┐┌{4:─^{1}}┐┌{5:─^{1}}┐\n"
    "│{6: ^{1}}││{7: ^{1}}││{8: ^{1}}││{9: ^{1}}│\n"
    "└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘\n", 
    "", 20, "A", "B", "C", "D", 
    is.kButtonStatus[keys.A], is.kButtonStatus[keys.B], is.kButtonStatus[keys.C], is.kButtonStatus[keys.D]);
  o<<s ;
  s = fmt::format(
    "┌{2:─^{1}}┐┌{3:─^{1}}┐┌{4:─^{1}}┐┌{5:─^{1}}┐\n"
    "│{6: ^{1}}││{7: ^{1}}││{8: ^{1}}││{9: ^{1}}│\n"
    "└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘\n", 
    "", 20, "E", "F", "Reserved", "Right", 
    is.kButtonStatus[keys.E], is.kButtonStatus[keys.F], is.kButtonStatus[keys.reserved], is.kButtonStatus[keys.right]);
  o<<s ;

  s = fmt::format(
    "┌{2:─^{1}}┐┌{3:─^{1}}┐┌{4:─^{1}}┐┌{5:─^{1}}┐\n"
    "│{6: ^{1}}││{7: ^{1}}││{8: ^{1}}││{9: ^{1}}│\n"
    "└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘\n",
    "", 20, "SW1", "SW2", "SW3", "SW4", 
    keys.sw1, keys.sw2, keys.sw3, keys.sw4);
  o<<s ;

  s = fmt::format(
    "┌{2:─^{1}}┐┌{3:─^{1}}┐┌{4:─^{1}}┐┌{5:─^{1}}┐\n"
    "│{6: ^{1}}││{7: ^{1}}││{8: ^{1}}││{9: ^{1}}│\n"
    "└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘\n", 
    "", 20, "left_axis_x", "left_axis_y", "right_axis_x", "right_axis_y", 
    keys.left_axis_x, keys.left_axis_y, keys.right_axis_x, keys.right_axis_y);
  o<<s ;
  return o;
}

