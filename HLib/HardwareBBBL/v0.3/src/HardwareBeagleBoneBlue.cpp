#include<iostream>
#include <chrono>
#include <thread>
#include <vector>

#include<rc/i2c.h>
#include<rc/time.h>
#include <rc/mpu.h>

#include"HardwareBeagleBoneBlue.hpp"

BeagleBoneBlueCurrentSensor::BeagleBoneBlueCurrentSensor()
    :_word_rx_buffer(0),_sign_conversion_buffer(0)
    {
    // Initiallize external I2C BUS, by default we took the INA260 0x40 default 
    // address
    if(rc_i2c_init(INA260::BUS_INFO::EXTERNAL_I2C_BUS, 
        INA260::BUS_INFO::INA260_RIGHT_MOTOR_ADDRESS)==0)
        initialize_sensors();
    else{
        /**
         * @todo Link a standar logger of Robot Status. It may be ROS... [ERROR]
         *       Unable to Start I2C BUS
         * @note A constructor cannot return a value
         */
        std::cerr<<"[ERROR] Unable to initialize external I2C BUS\n";
        //return EXIT_FAILURE; 
    }
}

BeagleBoneBlueCurrentSensor::~BeagleBoneBlueCurrentSensor(){
      
}

void BeagleBoneBlueCurrentSensor::initialize_sensors(){
    bool RSensor_is_running = false, LSensor_is_running = false;
    if(rc_i2c_set_device_address(INA260::BUS_INFO::EXTERNAL_I2C_BUS, 
       INA260::BUS_INFO::INA260_RIGHT_MOTOR_ADDRESS)==-1){
        std::cerr<<"[Error] Unable to reach Right Motor Current Sensor \n";
        return ;
    }

    RSensor_is_running = rc_i2c_write_word(INA260::BUS_INFO::EXTERNAL_I2C_BUS, 
                                      INA260::REGISTER_ADDRESS::CONFIGURATION,
                                  INA260::INA260_IV_READING_CONFIGURATION_N)==0;    
    if(rc_i2c_set_device_address(INA260::BUS_INFO::EXTERNAL_I2C_BUS, 
                    INA260::BUS_INFO::INA260_LEFT_MOTOR_ADDRESS)==-1){        
        std::cerr<<"[Error] Unable to reach Left Motor Current Sensor \n";
        return;
    }
    LSensor_is_running = rc_i2c_write_word(INA260::BUS_INFO::EXTERNAL_I2C_BUS,
                                        INA260::REGISTER_ADDRESS::CONFIGURATION,
                                  INA260::INA260_IV_READING_CONFIGURATION_N)==0;
    if( !LSensor_is_running ){
        std::cerr<<"[Error] Unable to configure Left Motor Current Sensor \n";
        return ;
    }
    if( !RSensor_is_running ){
        std::cerr<<"[Error] Unable to configure Right Motor Current Sensor \n";
        return ;
    }
    rc_usleep(5000);    
}

void BeagleBoneBlueCurrentSensor::get_left_motor_voltage(double& voltage){
    rc_i2c_set_device_address(INA260::BUS_INFO::EXTERNAL_I2C_BUS, 
                              INA260::BUS_INFO::INA260_LEFT_MOTOR_ADDRESS);
    rc_i2c_read_word(INA260::BUS_INFO::EXTERNAL_I2C_BUS,
                     INA260::REGISTER_ADDRESS::BUS_VOLTAGE,
                     &_word_rx_buffer);
    _sign_conversion_buffer = (int16_t) _word_rx_buffer;
    voltage = INA260::K_voltage_conversion * (double)(_sign_conversion_buffer);             
}

void BeagleBoneBlueCurrentSensor::get_left_motor_current(double& voltage){
    rc_i2c_set_device_address(INA260::BUS_INFO::EXTERNAL_I2C_BUS, 
                              INA260::BUS_INFO::INA260_LEFT_MOTOR_ADDRESS);
    rc_i2c_read_word(INA260::BUS_INFO::EXTERNAL_I2C_BUS,
                     INA260::REGISTER_ADDRESS::CURRENT,
                     &_word_rx_buffer);
    _sign_conversion_buffer = (int16_t) _word_rx_buffer;
    voltage = INA260::K_current_conversion * (double)(_sign_conversion_buffer);             
}

void BeagleBoneBlueCurrentSensor::get_right_motor_voltage(double& voltage){
    rc_i2c_set_device_address(INA260::BUS_INFO::EXTERNAL_I2C_BUS, 
                              INA260::BUS_INFO::INA260_RIGHT_MOTOR_ADDRESS);
    rc_i2c_read_word(INA260::BUS_INFO::EXTERNAL_I2C_BUS,
                     INA260::REGISTER_ADDRESS::BUS_VOLTAGE,
                     &_word_rx_buffer);
    _sign_conversion_buffer = (int16_t) _word_rx_buffer;
    voltage = INA260::K_voltage_conversion * (double)(_sign_conversion_buffer);             
}
void BeagleBoneBlueCurrentSensor::get_right_motor_current(double& voltage){
    rc_i2c_set_device_address(INA260::BUS_INFO::EXTERNAL_I2C_BUS, 
                              INA260::BUS_INFO::INA260_RIGHT_MOTOR_ADDRESS);
    rc_i2c_read_word(INA260::BUS_INFO::EXTERNAL_I2C_BUS,
                     INA260::REGISTER_ADDRESS::CURRENT,
                     &_word_rx_buffer);
    _sign_conversion_buffer = (int16_t) _word_rx_buffer;
    voltage = INA260::K_current_conversion * (double)(_sign_conversion_buffer);             
}

// IMU interruption routine
#define I2C_BUS 2
#define GPIO_INT_PIN_PIN 21
#define TAP_MAX_GAP_MS 800

static rc_mpu_data_t mpu_data;
static double yaw, pitch, roll;
static std::chrono::time_point<std::chrono::high_resolution_clock> last_tap_time =
  std::chrono::high_resolution_clock::now();
static int tap_count = 0;
static std::vector<int> taps;

static const char tap_codes[] = {
  'a', 'b', 'c', 'd', 'e',
  'f', 'g', 'h', 'i', 'j',
  'l', 'm', 'n', 'o', 'o',
  'q', 'r', 's', 't', 'u',
  'v', 'w', 'x', 'y', 'z'
};
static char tap_code = '\0';

static void on_imu_changed() {
  yaw = mpu_data.dmp_TaitBryan[TB_YAW_Z] * RAD_TO_DEG;
  pitch = mpu_data.dmp_TaitBryan[TB_PITCH_X] * RAD_TO_DEG;
  roll = mpu_data.dmp_TaitBryan[TB_ROLL_Y] * RAD_TO_DEG;
  if (tap_count > 0) {
    std::chrono::time_point<std::chrono::high_resolution_clock> now =
      std::chrono::high_resolution_clock::now();
    int elapsed_millis = std::chrono::duration_cast<std::chrono::milliseconds>
      (now - last_tap_time).count();
    if (elapsed_millis > TAP_MAX_GAP_MS) {
      taps.push_back(tap_count - 1);
      tap_count = 0;
      if (taps.size() == 2) {
        tap_code = tap_codes[taps[0] * 5 + taps[1]];
        taps.clear();
        //logger::info("Tap code: %c", tap_code);
      }
    }
  }
}
static void tap_callback(int direction, int counter) {
  tap_count++;
  last_tap_time = std::chrono::high_resolution_clock::now();
  //logger::info("Tap direction: %d, count: %d", direction, counter);
}

BeagleBoneBlueIMU::BeagleBoneBlueIMU() {
  rc_mpu_config_t mpu_config = rc_mpu_default_config();
  mpu_config.dmp_sample_rate = 200;
  mpu_config.dmp_fetch_accel_gyro = 1;  // provide raw data too
  mpu_config.tap_threshold = 5;
  mpu_config.orient = ORIENTATION_X_UP;
  mpu_config.i2c_bus = I2C_BUS;
  mpu_config.gpio_interrupt_pin_chip = GPIO_INT_PIN_CHIP;
  mpu_config.gpio_interrupt_pin = GPIO_INT_PIN_PIN;
  if (rc_mpu_initialize_dmp(&mpu_data, mpu_config)) {
    //logger::error("Can't initialize inertial measurement unit");
  } else {
    rc_mpu_set_dmp_callback(&on_imu_changed);
    //rc_mpu_set_tap_callback(&tap_callback);
  }
}

BeagleBoneBlueIMU::~BeagleBoneBlueIMU() {
  rc_mpu_power_off();
}

double BeagleBoneBlueIMU::get_yaw() {
  return yaw;
}

double BeagleBoneBlueIMU::get_pitch() {
  return pitch;
}

double BeagleBoneBlueIMU::get_roll() {
  return roll;
}

char BeagleBoneBlueIMU::get_tap_code() {
  char code = tap_code;
  tap_code == '\0';  // consume code on read
  return code;
}