#ifndef SRC_INA260_HPP
#define SRC_INA260_HPP

namespace ina260 {
/** 
 * REGISTER ADDRESSES
 *
 * \brief INA 260 Register Addresses
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
const uint16_t INA260_CURRENT_READING_CONFIGURATION_N = CONFIGURATION_REGISTER::PREFFIX | CONFIGURATION_REGISTER::AVG_MOD_1S | 
                                        CONFIGURATION_REGISTER::VBUSCT_1100us | CONFIGURATION_REGISTER::ISHCT_1100us |
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
#define EXTERNAL_I2C_BUS 1 

const uint8_t K_DEFAULT_RIGHT_MOTOR_ADDRESS_ = 0x41;
const uint8_t K_DEFAULT_LEFT_MOTOR_ADDRESS_ = 0x40;

const float K_current_conversion = 1.25e-3;
const float K_voltage_conversion = 1.25e-3;

/**
 * @brief      Initialize INA260 sensor. The external I2C peripherial needs to be initialized beforehand.
 *
 * @param[in] address Address from (0x40 to 0x45). 
 *                    If no parameter is provided the default address 0x40
 *                     will be used by default. 
 *
 * @return     0 on success, -1 on failure
 */
int init(const uint8_t address);
int init(void);
/**
 * @brief  Configure INA260 sensor. Referr to  
 *
 * @param[in] address Address from (0x40 to 0x45). 
 *                    If no parameter is provided the default address 0x40
 *                     will be used by default.
 * @param[in] configuration Referr to https://www.ti.com/lit/ds/symlink/ina260.pdf?ts=1651307719003 p22. to determine 
 *                          the configuration word. Register enum types are provided.
 *                          If no configuration word is provided fast current reading will be used. 
 *                          INA260_CURRENT_READING_CONFIGURATION_H 
 *
 * @return     0 on success, -1 on failure
 */
int configure_sensor(uint8_t address, uint16_t configuration);
int configure_sensor(uint16_t configuration);
int configure_sensor(void);
/**  
 * @brief  Reads the current register of INA260 sensor and returns the current value in [A](amps).
 *         The sensor needs to be properly configured.
 *
 * @param[in] address Address from (0x40 to 0x45). 
 *                    If no parameter is provided the default address 0x40
 *                     will be used by default.
 *
 * @return     Sensor current in amperes on the range [-42, 42]. 
 *             No reading status is provided, then value must be tested for validity.
 */
double read_current(uint8_t address);
double read_current(void);
/** 
 * @brief  Reads the voltage register of INA260 sensor and returns the current value in [V](volts).
 *         The sensor needs to be properly configured.
 *
 * @param[in] address Address from (0x40 to 0x45). 
 *                    If no parameter is provided the default address 0x40
 *                     will be used by default.
 *
 * @return     Sensor voltage in amperes on the range [-42, 42]. 
 *             No reading status is provided, then value must be tested for validity.
 */
double read_voltage(uint8_t address);
double read_voltage(void);


}




#endif // SRC_INA260_HPP