#ifdef  DRV8871
#define DRV8871

int motor_init_freq(const int &pwm_frequency_hz);
/**
 * @brief Initialization of the two motor channels  in free spin. 
 *
 * This function starts all motors at MAX_PWM_FREQ of 20,000kHz 
 * 
 *  @param[in]  pwm_frequency_hz  The pwm frequency in hz
 *
 * @return     0 on success, -1 on failure which is usually due to lack of user
 * permissions to access the gpio and pwm systems.
 * 
 */

int motor_set_ch( const int &channel, const double &duty_cycle);
/**
 * @brief Function to drive a motor at a given duty cycle.
 * 
 * This function operates the given drv8871 at the desired duty cicle.
 * 
 * @param[in] channel The channel to be driven (1 or 2).
 * @param[in] duty_cycle Desired duty cycle given as a number in the range of
 *                       [-1, 1]. Direction is determined from the sign of the
 *                       duty_cycle. 
 * @return   0 on success, -1 on failure which is usually due to lack of user
 * permissions to access the gpio and pwm systems.
 */

void motor_clean_up(void);
/**
 * @brief 
 * 
 */ 
#endif // DRV8871