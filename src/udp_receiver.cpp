#include <cstring>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "udp_gamepad/udp_receiver.h"

namespace udp_gamepad {
/**
 * @brief Constructor for UdpReceiver.
 * @param port The port to listen for UDP data.
 */
UdpReceiver::UdpReceiver(int port) : socket_fd_(-1) {
  socket_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_fd_ == -1) {
    std::cerr << "Failed to create socket." << std::endl;
    return;
  }

  sockaddr_in serverAddress{};
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(port);

  if (bind(socket_fd_, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(serverAddress)) < 0) {
    std::cerr << "Failed to bind socket." << std::endl;
    close(socket_fd_);
    return;
  }
}

/**
 * @brief Receive UDP data.
 * @return A vector containing received data as uint8_t.
 */
std::vector<uint8_t> UdpReceiver::ReceiveData() {
  std::vector<uint8_t> buffer(1024);
  sockaddr_in clientAddress{};
  socklen_t clientAddressLen = sizeof(clientAddress);

  int bytesRead = recvfrom(socket_fd_,
                           buffer.data(),
                           buffer.size(),
                           0,
                           reinterpret_cast<sockaddr *>(&clientAddress),
                           &clientAddressLen);
  if (bytesRead == -1) {
    std::cerr << "Failed to receive data." << std::endl;
    return {};
  }

  buffer.resize(bytesRead);

  return buffer;
}

/**
 * @brief Destructor for UdpReceiver.
 */
UdpReceiver::~UdpReceiver() {
  if (socket_fd_ != -1) {
    close(socket_fd_);
  }
}
} // namespace udp_gamepad
