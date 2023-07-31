#ifndef SRC_HARDWARE_HPP_
#define SRC_HARDWARE_HPP_

#include <functional>
#include <vector>

namespace hardware{
  class CurrentSensor{
    public:
        BeagleBoneBlueCurrentSensor();
        virtual ~BeagleBoneBlueCurrentSensor();
        void get_left_motor_current (double& current) override;
        void get_right_motor_current (double& current) override;
        void get_left_motor_voltage (double& voltage) override;
        void get_right_motor_voltage (double& voltage) override;
    private:
        void initialize_sensors();
        uint16_t _word_rx_buffer;
        int16_t _sign_conversion_buffer;
};
class IMU{
 public:
  BeagleBoneBlueIMU();
  virtual ~BeagleBoneBlueIMU();
  
  double get_pitch();
  double get_roll();
  char get_tap_code();
};

} // namespace hardware
#endif  // SRC_HARDWARE_HPP_
