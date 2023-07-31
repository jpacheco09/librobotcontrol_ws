#ifndef SRC_PCA9685_HPP
#define SRC_PCA9685_HPP

namespace pca9685{
    enum REGISTER_ADDRESS : std::uint8_t {
    MODE1      = 0x00,
    MODE2      = 0X01,
    LED0_ON_L  = 0X06,
    LED0_ON_H  = 0x07,
    LED0_OFF_L = 0x08,
    LED0_OFF_H = 0x09,
    // etc all 16:  LED15_OFF_H 0x45
    ALLLED_ON_L   = 0xFA, /**< load all the LEDn_ON registers, low */
    ALLLED_ON_H   = 0xFB, /**< load all the LEDn_ON registers, high */
    ALLLED_OFF_L  = 0xFC, /**< load all the LEDn_OFF registers, low */
    ALLLED_OFF_H  = 0xFD, /**< load all the LEDn_OFF registers,high */
    PRESCALE 	  = 0xFE, /**< Prescaler for PWM output frequency */
    TESTMODE 	  = 0xFF /**< defines the test mode to be entered */
    };

    enum MODE1 : std::uint8_t{
    ALLCAL =   0x01,  /**< respond to LED All Call I2C-bus address */
    SUB3   =   0x02,  /**< respond to I2C-bus subaddress 3 */
    SUB2   =   0x04,  /**< respond to I2C-bus subaddress 2 */
    SUB1   =   0x08,  /**< respond to I2C-bus subaddress 1 */
    SLEEP  =   0x10,  /**< Low power mode. Oscillator off */
    AI     =   0x20,  /**< Auto-Increment enabled */
    EXTCLK =   0x40,  /**< Use EXTCLK pin clock */
    RESTART=   0x80,  /**< Restart enabled */     
    DEFAULT=   0b00110000u  
    };

    enum MODE2 : std::uint8_t{
    OUTNE_0      =   0x01,  /**< Active LOW output enable input */
    OUTNE_1      =   0x02,  /**< Active LOW output enable input - high impedience */
    OUTDRV       =   0x04,  /**< totem pole structure vs open-drain */
    OCH          =   0x08,  /**< Outputs change on ACK vs STOP */
    INVRT        =   0x10,  /**< Output logic state inverted */
    DEFAUL      =   0b00000100u  
    };
    #define EXTERNAL_I2C_BUS 1
    const uint8_t default_address = 0x42;
    /**
     * @brief      Initialize INA260 sensor. The external I2C peripherial needs to be initialized beforehand.
     *
     * @param[in] address Address from (0x40 to 0x45). 
     *                    If no parameter is provided the default address 0x42
     *                     will be used by default. 
     *
     * @return     0 on success, -1 on failure
     */
    int init(uint8_t address);
    /**
     * @brief      Wakes up PC9685 module
     *
     * @param[in] address Address from (0x40 to 0x45). 
     *                    If no parameter is provided the default address 0x42
     *                     will be used by default. 
     *
     * @return     0 on success, -1 on failure
     */
    bool wakeup(void);
    /**
     * @brief      Modifies board preescaler 
     *
     * @param[in] address Address from (0x40 to 0x45). 
     *                    If no parameter is provided the default address 0x42
     *                     will be used by default. 
     *
     * @return     true on success, false on failure
     */
    bool set_pwm_frequency(void);
    /**
     * @brief      Modifies board preescaler 
     *
     * @param[in] address Address from (0x40 to 0x45). 
     *                    If no parameter is provided the default address 0x42
     *                     will be used by default. 
     *
     * @return     true on success, false on failure
     */
    int is_sleeping(bool *sleeping);
    /**
     * @brief      modifies n-th input to desired duty cycle 
     *
     * @param[in] address Address from (0x40 to 0x45). 
     *                    If no parameter is provided the default address 0x42
     *                     will be used by default. 
     * @param[in] channel Output Channel from 0 to 15. 
     * 
     * @param[in] duty_cycle Output duty cycle from [0 to 1]. 
     * @return     0 on success, -1 on failure
     */
    int set_channel_duty_cycle(uint8_t address,uint8_t channel, float duty_cycle);
    /**
     * @brief      modifies n-th input to desired duty cycle 
     *
     * @param[in] address Address from (0x40 to 0x45). 
     *                    If no parameter is provided the default address 0x42
     *                     will be used by default. 
     * @param[in] channel Output Channel from 0 to 15. 
     * 
     * @param[in] on_time on time from 0 to 4095. 
     * 
     * @param[in] off_time on time from 0 to 4095.
     * @return     0 on success, -1 on failure
     */
    int set_channel_pwm_times(uint8_t address, uint8_t channel, uint16_t on_time, uint16_t off_time);    
} //namespace pca9685

/**
 * 
 * @TODO We need to implement a closing routine, such that we can guarantee that there's no PWM signals sent
 *       if there's any potential risks or a stop condition.
 * @TODO Implement a function that allows to update alll channel motors at once. 
 *       If such scenario is possible, we'll be able to update the motor action without unwanted transient responses.
 */
#endif // SRC_PCA9685_HPP
