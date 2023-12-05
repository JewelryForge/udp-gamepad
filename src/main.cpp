#include <iostream>
// #include "gamepad/gamepad product.h"

#include "gamepad.h"
#include "retroid_gamepad.h"
#include "skydroid_gamepad.h"

#include "udp_receiver.h"
#include "gamepad_keys.h"


int main(int argc, char* argv[]) {
  RetroidGamepad rc(42121);
  const RetroidKeys *keys = &rc.GetKeys();
  // SkydroidGamepad rc(43897);
  // const SkydroidKeys *keys = &rc.GetKeys();

  rc.updateCallback_ = [&](int32_t count){
    std::cout << rc << std::endl;
  };
    
  for(int i = 0; i < 100000; i ++){
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 0;
}
