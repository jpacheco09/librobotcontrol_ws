#ifndef SRC_HARDWAREBEAGLEBONEBLUE_HPP_
#define SRC_HARDWAREBEAGLEBONEBLUE_HPP_

#define GPIO_INT_PIN_CHIP 3

namespace hardware{
enum class Joint { left_wheel, right_wheel };
struct JointPosition {
  Joint joint;
  double degrees;
};

class JointState {
 public:
  /**
   * In radians.
   */
  double position;

  /**
   * Revolutions per minute.
   */
  double rpm;

  /**
   * In amperes.
   */
  double current;
};

class IMU {
 public:
  virtual double get_yaw() = 0;
  virtual double get_pitch() = 0;
  virtual double get_roll() = 0;
  virtual char get_tap_code() = 0;
};

class Current_Sensor {
    private:
    void init();
 public:
 /**
  * @brief Get the left motor current object
  * 
  * @return double 
  */
  virtual void get_left_motor_current(double& current) = 0;
  virtual void get_right_motor_current(double& current) = 0;
  virtual void get_left_motor_voltage(double& voltage) = 0;
  virtual void get_right_motor_voltage(double& voltage) = 0;  
};

} // end hardware namespace

namespace motor{
  /**
   * @brief Relevant definitions for the Pololu 37D 24v 141:1 DC motors
   * Most parameters are taken from the official datashet linked below. 
   * According to a pololu engineer (https://forum.pololu.com/t/mechanics-and-electrical-parameters/18153), resistance can be determined as a quotient between the stall current and the rated voltage:
   * I_stall = 2.6 [A],  V_rated = 24 [V] then 
   * R_armor =  V_rated / I_stall = 24 / 2.6 [Ohm] = 9.23 [Ohm] %meassured 17 [ohm]
   * Inductance for this motor series is around 2.4mH
   * L_armor = 2.4mH
   * t_e = L_armor / R_armor = 2.6 ms
   * The electromotive force constant (Ke) can be approximated by dividing the rated voltage by the free-run speed (at the rated voltage).
   * V_rated = 24 [V], w_fr = 79 [rpm],
   * K_e = 24 / 79 [V/RPM] = 0.3038 
   * approximate the motor torque constant (Kt), you can divide the stall torque by the stall current.
   * I_stall = 2.6 [A], \tau_{stall} = 470 [kg mm]  = 4.609 [Nm] (rule of thumb: keept it at 30%: 1.3827[Nm]~=141[kg mm])
   * K_t = 4.609 / 2.6  = 1.77726 [Nm/A] , but for realistic application
   * K_t =  1.3827/0.87  = 1.5893 [Nm/A]
   * @link https://www.pololu.com/file/0J1736/pololu-37d-metal-gearmotors-rev-1-2.pdf @endlink
   * 
   */
  #define right_wheel_encoder_polarity 1
  #define left_wheel_encoder_polarity 1
  #define right_wheel_gear_box 131
  #define left_wheel_gear_box 131
  #define right_wheel_encoder_resolution 64
  #define left_wheel_encoder_resolution 64
  // stall current: 2.6
  #define datasheet_inductance 2.43e-3 // [H]
  #define datasheet_resistance 9.23 // [ohm]
  #define datasheet_torque_constant 1.5893

  #define right_wheel_encoder_port 1
  #define left_wheel_encoder_port 2

  /*enum DATASHEET_ELECTRICAL_PARAMETERS: double {
    inductance = 2.43e-3; //[H]
    resistance = 9.23; // [ohm]
    torque_constant = 1.5893; // [Nm/A]
    back_emf_constant = 0.3038 // [V/RPM];
  };*/
  /*enum RIGHT_ENCODER_PARAMETERS: int {
    polarity = 1; 
    gear_box = 131;
    resolution = 64; 
  };
  enum LEFT_ENCODER_PARAMETERS: int {
    polarity = 1; 
    gear_box = 131;
    resolution = 64; 
  };
  enum WIRING_DEFINITIONS: int{
    right_encoder_port = 1;
    left_encoder_port = 2;
    right_motor_pwm_port = 1;
    left_motor_pwm_port = 2;
  };*/


}

namespace INA260 {
/** 
 * REGISTER ADDRESSES
 *
 * \brief Register Addresses enum class
 *
 * \rst
 * There is a :math:`N^2` environment for reStructuredText!
 *
 * +========================+===================+
 * | Register Name          |   Addres(HEX)     |
 * +========================+===================+
 * | Configuration Register |       0x00        |
 * +------------------------+-------------------+
 * |    Current Register    |       0x01        |
 * +------------------------+-------------------+
 * |  Bus Voltage Register  |       0x02        |
 * +------------------------+-------------------+
 * |    Power Register      |       0x03        |
 * +------------------------+-------------------+
 * |  Mask/Enable Register  |       0x06        |
 * +------------------------+-------------------+
 * |  Alert Limit Register  |       0x07        |
 * +------------------------+-------------------+
 * |Manufacturer ID Register|       0xFE        |
 * +------------------------+-------------------+
 * |    Die ID Register     |       0xFF        |
 * +========================+===================+
 * \endrst
*/
enum REGISTER_ADDRESS : std::uint8_t {
    CONFIGURATION   = 0x00,
    CURRENT         = 0X01,
    BUS_VOLTAGE     = 0X02,
    POWER           = 0X03,
    MASK_ENABLE     = 0x06,
    ALERT_LIMIT     = 0x07,
    MANUFACTURER_ID = 0xFE,
    DIE_ID          = 0xFF
};
/**
 * 
 * @brief Brief enum declaration of Configuration main functionalities
 * Configuration register distribution
 * +=========+=========+=========+=========+=========+=========+=========+=========+=========+=========+=========+=========+=========+=========+=========+=========+
 * * |  15   |   14    |   13    |   12    |   11    |   10    |    9    |    8    |    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
 * +---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+
 * |   RST   |    -    |    -    |    -    |  AVG2   |  AVG1   |  AVG0   | VBUSCT2 | VBUSCT1 | VBUSCT0 | ISHCT2  | ISHCT1  | ISHCT0  |  MODE3  |  MODE2  |  MODE1  |
 * +---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+
 * |    0    |    1    |    1    |    0    |    0    |    0    |    1    |    0    |    1    |    1    |    0    |    1    |    1    |    1    |    1    |    1    |
 * +---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+
 * |                                       |                   2                   |                   D                   |                   F                   |
 * +=========+=========+=========+=========+=========+=========+=========+=========+=========+=========+=========+=========+=========+=========+=========+=========+
 * 
 */
enum CONFIGURATION_REGISTER : std::uint16_t{    
    RST     = 0x8000, PREFFIX = 0x6000,
    AVG2    = 0x0800, AVG1 = 0x0400, AVG0 = 0x0200,
    VBUSCT2 = 0x0100, VBUSCT1 = 0x0080, VBUSCT0 = 0x0040,
    ISHCT2  = 0x0020, ISHCT1  = 0x0010, ISHCT0  = 0x0008,
    MODE3   = 0x0004, MODE2   = 0x0002, MODE1   = 0x0001,    
    AVG_MOD_1S = 0x0000, AVG_MOD_4S = 0x0000 | AVG0, AVG_MOD_16S = AVG1,
    AVG_MOD_64S = 0&AVG2 | AVG1 | AVG0, AVG_MOD_128S = AVG2| 0&AVG1 | 0&AVG0, AVG_MOD_256S = AVG2|  0&AVG1 | AVG0,  
    AVG_MOD_512S = AVG2 | AVG1 | 0&AVG0, AVG_MOD_1024S = AVG2 | AVG1 | AVG0,
    VBUSCT_140us = 0x0000,VBUSCT_204us = 0&VBUSCT2 | 0&VBUSCT1 | VBUSCT0, VBUSCT_332us = 0&VBUSCT2 | VBUSCT1 | 0&VBUSCT0,
    VBUSCT_588us = 0&VBUSCT2 | VBUSCT1 | VBUSCT0, VBUSCT_1100us = VBUSCT2 | 0&VBUSCT1 | 0&VBUSCT0,VBUSCT_2116us = VBUSCT2 | 0&VBUSCT1 | VBUSCT0, 
    VBUSCT_4156us = VBUSCT2 | VBUSCT1 | 0&VBUSCT0,VBUSCT_8244us = VBUSCT2 | VBUSCT1 | VBUSCT0,
    ISHCT_140us = 0x0000,ISHCT_204us = 0&ISHCT2 | 0&ISHCT1 | ISHCT0, ISHCT_332us = 0&ISHCT2 | ISHCT1 | 0&ISHCT0,
    ISHCT_588us = 0&ISHCT2 | ISHCT1 | ISHCT0, ISHCT_1100us = ISHCT2 | 0&ISHCT1 | 0&ISHCT0,ISHCT_2116us = ISHCT2 | 0&ISHCT1 | ISHCT0, 
    ISHCT_4156us = ISHCT2 | ISHCT1 | 0&ISHCT0,ISHCT_8244us = ISHCT2 | ISHCT1 | ISHCT0,
    MODE_Co_IV = MODE3 | MODE2 | MODE1, MODE_Co_V = MODE3 | MODE2 | 0&MODE1, MODE_Co_I = MODE3 | 0&MODE2 | MODE1, MODE_Co_PD = MODE3 | 0&MODE2 | 0&MODE1,
    MODE_Tr_IV = 0&MODE3 | MODE2 | MODE1, MODE_Tr_V = 0&MODE3 | MODE2 | 0&MODE1, MODE_Tr_I = MODE3 | 0&MODE2 | MODE1, MODE_Tr_PD = 0&MODE3 | 0&MODE2 | 0&MODE1,
    };
const uint16_t INA260_CURRENT_READING_CONFIGURATION_N = CONFIGURATION_REGISTER::PREFFIX | CONFIGURATION_REGISTER::AVG_MOD_4S | 
                                        CONFIGURATION_REGISTER::VBUSCT_588us | CONFIGURATION_REGISTER::ISHCT_588us |
                                        CONFIGURATION_REGISTER::MODE_Co_I;
const uint16_t INA260_CURRENT_READING_CONFIGURATION_H = CONFIGURATION_REGISTER::PREFFIX | CONFIGURATION_REGISTER::AVG_MOD_4S | 
                                        CONFIGURATION_REGISTER::VBUSCT_140us | CONFIGURATION_REGISTER::ISHCT_140us |
                                        CONFIGURATION_REGISTER::MODE_Co_I;
const uint16_t INA260_VOLTAGE_READING_CONFIGURATION_N = CONFIGURATION_REGISTER::PREFFIX | CONFIGURATION_REGISTER::AVG_MOD_4S | 
                                        CONFIGURATION_REGISTER::VBUSCT_588us | CONFIGURATION_REGISTER::ISHCT_588us |
                                        CONFIGURATION_REGISTER::MODE_Co_V;    
const uint16_t INA260_IV_READING_CONFIGURATION_N = CONFIGURATION_REGISTER::PREFFIX | CONFIGURATION_REGISTER::AVG_MOD_4S | 
                                        CONFIGURATION_REGISTER::VBUSCT_588us | CONFIGURATION_REGISTER::ISHCT_588us |
                                        CONFIGURATION_REGISTER::MODE_Co_IV;                                          
enum BUS_INFO : std::uint8_t{
    EXTERNAL_I2C_BUS = 1, 
    INA260_RIGHT_MOTOR_ADDRESS = 0x41,
    INA260_LEFT_MOTOR_ADDRESS = 0x40
};

const float K_current_conversion = 1.25e-3;
const float K_voltage_conversion = 1.25e-3;
}


class BeagleBoneBlueCurrentSensor:public hardware::Current_Sensor{
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

class BeagleBoneBlueIMU: public hardware::IMU {
 public:
  BeagleBoneBlueIMU();
  virtual ~BeagleBoneBlueIMU();
  double get_yaw();
  double get_pitch();
  double get_roll();
  char get_tap_code();
};


#endif  // SRC_HARDWAREBEAGLEBONEBLUE_HPP_