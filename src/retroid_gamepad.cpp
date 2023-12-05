#include "retroid_gamepad.h"
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <bitset>


/**
 * @brief Constructor for the RetroidGamepad class.
 *
 * Initializes the Retroid gamepad with the specified UDP port for data reception.
 *
 * @param port The UDP port to use for receiving gamepad data.
 */
RetroidGamepad::RetroidGamepad(int port) : Gamepad(port) {

  std::string pressed  = fmt::format("\x1b[32m{:^20}\x1b[0m", "pressed");
  std::string released = fmt::format("\x1b[37m{:^20}\x1b[0m", "released");
  kButtonStatus.push_back(released);
  kButtonStatus.push_back(pressed);

}

/**
 * @brief Updates the RETROID gamepad data buffer and keys.
 *
 * @param buffer The data buffer to be updated.
 * @param keys The RETROID gamepad keys to be updated.
 * @return True if the data is valid and updated, false otherwise.
 */
bool RetroidGamepad::UpdateData(std::vector<uint8_t>& buffer, RetroidKeys& keys) {
  RetroidGamepadData data;
  memcpy(&data, buffer.data(), buffer.size()*sizeof(uint8_t));
  // Perform data validity check in the child class
  if (DataIsValid(data)) {
    std::bitset<kChannlSize> value_bit(0);
    int16_t ch[kChannlSize];
    memcpy(ch, data.data, sizeof(ch));
    for(int i = 0; i < kChannlSize; i++){
      value_bit[i] = ch[i];
    }
    keys.value = value_bit.to_ulong();

    keys.left  = (data.left_axis_x == -kJoystickRange) ? (uint8_t)KeyStatus::kPressed 
                  : ((data.left_axis_x ==  kJoystickRange) ? (uint8_t)KeyStatus::kReleased : (uint8_t)KeyStatus::kReleased);
    keys.right = (data.left_axis_x ==  kJoystickRange) ? (uint8_t)KeyStatus::kPressed 
                  : ((data.left_axis_x == -kJoystickRange) ? (uint8_t)KeyStatus::kReleased : (uint8_t)KeyStatus::kReleased);
    keys.up    = (data.left_axis_y ==  kJoystickRange) ? (uint8_t)KeyStatus::kPressed 
                  : ((data.left_axis_y == -kJoystickRange) ? (uint8_t)KeyStatus::kReleased : (uint8_t)KeyStatus::kReleased);
    keys.down  = (data.left_axis_y == -kJoystickRange) ? (uint8_t)KeyStatus::kPressed 
                  : ((data.left_axis_y ==  kJoystickRange) ? (uint8_t)KeyStatus::kReleased : (uint8_t)KeyStatus::kReleased);

    keys.left_axis_x = data.left_axis_x/(float)kJoystickRange;
    keys.left_axis_y = data.left_axis_y/(float)kJoystickRange;
    keys.right_axis_x = data.right_axis_x/(float)kJoystickRange;
    keys.right_axis_y = data.right_axis_y/(float)kJoystickRange;

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
bool RetroidGamepad::DataIsValid(const RetroidGamepadData& data) {

  // 1. check STX   
  if (data.stx[0] != kHeader[0] || data.stx[1] != kHeader[1]) {
    return false;
  }

  // 2. check device ID
  if (data.id != (uint8_t)GamepadType::kRetroid) {
    return false;
  }
  
  // 3. CRC16
  if(data.crc16 != CalculateCrc16(data.data, data.data_len)){
    return false;
  }

  return true;  
}

std::ostream& operator<<(std::ostream& o, RetroidGamepad& is){
  
  RetroidKeys keys = is.GetKeys();
  o << "\033c";
  std::string  s = fmt::format(
	"┌{0:─^{1}}┐\n"
	"│{2: ^{1}}│\n"
	"└{0:─^{1}}┘\n", "", 86 ,"Joystick Device: Retroid(Lite3)");
  o<<s ;
  s = fmt::format(
    "┌{2:─^{1}}┐┌{3:─^{1}}┐┌{4:─^{1}}┐┌{5:─^{1}}┐\n"
    "│{6: ^{1}}││{7: ^{1}}││{8: ^{1}}││{9: ^{1}}│\n"
    "└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘\n", 
    "", 20, "L2", "L1", "R1", "R2", 
    is.kButtonStatus[keys.L2], is.kButtonStatus[keys.L1], is.kButtonStatus[keys.R1], is.kButtonStatus[keys.R2]);
  o<<s ;
  s = fmt::format(
    "┌{2:─^{1}}┐┌{3:─^{1}}┐┌{4:─^{1}}┐┌{5:─^{1}}┐\n"
    "│{6: ^{1}}││{7: ^{1}}││{8: ^{1}}││{9: ^{1}}│\n"
    "└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘\n", 
    "", 20, "X", "Y", "A", "B", 
    is.kButtonStatus[keys.X], is.kButtonStatus[keys.Y], is.kButtonStatus[keys.A], is.kButtonStatus[keys.B]);
  o<<s ;

  s = fmt::format(
    "┌{2:─^{1}}┐┌{3:─^{1}}┐┌{4:─^{1}}┐┌{5:─^{1}}┐\n"
    "│{6: ^{1}}││{7: ^{1}}││{8: ^{1}}││{9: ^{1}}│\n"
    "└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘\n",
    "", 20, "up", "down", "left", "right", 
    is.kButtonStatus[keys.up], is.kButtonStatus[keys.down], is.kButtonStatus[keys.left], is.kButtonStatus[keys.right]);
  o<<s ;



  s = fmt::format(
    "┌{2:─^{1}}┐┌{3:─^{1}}┐┌{4:─^{1}}┐┌{5:─^{1}}┐\n"
    "│{6: ^{1}}││{7: ^{1}}││{8: ^{1}}││{9: ^{1}}│\n"
    "└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘\n", 
    "", 20, "select", "start", "left_axis_button", "right_axis_button", 
    is.kButtonStatus[keys.select], is.kButtonStatus[keys.start], is.kButtonStatus[keys.left_axis_button], is.kButtonStatus[keys.right_axis_button]);
  // fmt::print(s);
  o<<s ;
  s = fmt::format(
    "┌{2:─^{1}}┐┌{3:─^{1}}┐┌{4:─^{1}}┐┌{5:─^{1}}┐\n"
    "│{6: ^{1}}││{7: ^{1}}││{8: ^{1}}││{9: ^{1}}│\n"
    "└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘└{0:─^{1}}┘\n", 
    "", 20, "left_axis_x", "left_axis_y", "right_axis_x", "right_axis_y", 
    keys.left_axis_x, keys.left_axis_y, keys.right_axis_x, keys.right_axis_y);
  o<<s ;
  // o<<str_button ;
  return o;
}

