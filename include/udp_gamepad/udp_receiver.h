#ifndef UDP_RECEIVER_H
#define UDP_RECEIVER_H

#include <cstdint>
#include <vector>

namespace udp_gamepad {
/**
 * @brief The UdpReceiver class for receiving UDP data.
 */
class UdpReceiver {
public:
  /**
   * @brief Constructor for UdpReceiver.
   * @param port The port to listen for UDP data.
   */
  explicit UdpReceiver(int port);

  /**
   * @brief Receive UDP data.
   * @return A vector containing received data as uint8_t.
   */
  std::vector<uint8_t> ReceiveData();

  /**
   * @brief Destructor for UdpReceiver.
   */
  ~UdpReceiver();

private:
  int socket_fd_;
};
} // namespace udp_gamepad


#endif // UDP_RECEIVER_H
