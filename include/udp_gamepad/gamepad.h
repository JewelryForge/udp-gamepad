#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <atomic>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "gamepad_keys.h"

namespace udp_gamepad {
/**
 * @brief A template class for a generic Gamepad.
 *
 * This class serves as a generic gamepad template. It can be specialized for specific gamepad types.
 *
 * @tparam KeysType The type of keys for the gamepad (e.g., XboxKeys, PlayStationKeys).
 */
template<typename KeysType>
class Gamepad {
public:
  using Keys = KeysType;
  using Callback = std::function<void(KeysType, uint32_t)>;

  std::vector<std::string> button_status_;
  /**
   * @brief Constructor for the Gamepad class.
   *
   * Initializes the gamepad with the specified UDP port for data reception.
   *
   * @param port The UDP port to use for receiving gamepad data.
   */
  Gamepad(int port = kDefaultPort);

  /**
   * @brief Destructor for the Gamepad class.
   *
   * This destructor stops the data thread to clean up resources when the gamepad instance is destroyed.
   */
  virtual ~Gamepad() {
    StopDataThread();
  }

  /**
   * @brief Starts the data thread for simulating gamepad data.
   */
  void StartDataThread();

  /**
   * @brief Stops the data thread.
   */
  void StopDataThread();

  /**
   * @brief Gets the gamepad keys.
   * @return The current gamepad keys.
   */
  KeysType GetKeys() const;

  /**
   * @brief Sets a callback function to be called on each data update.
   * @param callback The callback function that takes an update count as an argument.
   */
  void SetUpdateCallback(const Callback &callback);

  /**
   * @brief Calculate the CRC-16 checksum for a given data buffer.
   *
   * @param data Pointer to the data buffer.
   * @param length Length of the data buffer.
   * @return The CRC-16 checksum of the data.
   */
  uint16_t CalculateCrc16(const uint8_t *data, size_t length);

protected:
  std::thread data_thread_;       ///< Thread for generating simulated data.
  std::atomic<bool> stop_thread_; ///< Flag to signal the data thread to stop.
  mutable std::mutex mutex_;      ///< Mutex for data access synchronization.
  KeysType keys_;                 ///< Gamepad keys.
  int port_;                      ///< The UDP port for data reception.
  timespec start_time_;           ///< receive timestamp start time
  Callback callback_;             ///< Callback function for data updates.
  uint32_t tick_{};               ///< message count

  /**
   * @brief Updates the gamepad data buffer and keys.
   *
   * @param buffer The data buffer to be updated.
   * @return True if the data is valid and updated, false otherwise.
   */
  virtual bool UpdateData(std::vector<uint8_t> &buffer) = 0;
};
} // namespace udp_gamepad


#endif // GAMEPAD_H
