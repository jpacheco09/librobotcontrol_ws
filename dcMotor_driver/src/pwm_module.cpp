#include<iostream>
#include<signal.h>
#include<assert.h>

#include<robotcontrol.h>

#include <Logger.hpp>

#define US_SECOND 1000000

static int running = 0;
// interrupt handler ctrl-c
static void __signal_handler(__attribute__ ((unused)) int dummy){
    running = 0;
    rc_set_state(EXITING);
}
//======================== PCA9685 definitions ================================= 
// based on https://github.com/henriheimann/stm32-hal-pca9685/blob/master/pca9685.c

#define MODE1_RESTART 0x80 
#define EXTERNAL_I2C_BUS 1
#define PCA9685_I2C_ADDRESS 0x42 /**< Board address. Default is 0x40*/
#define PCA9685_PRESCALE_MIN 3    
#define MODE1_AI 0x20      /**< Auto-Increment enabled */

// REGISTER ADDRESSES
#define PCA9685_MODE1 0x00 /**< Mode Register 1 */
#define PCA9685_MODE2 0x01 
#define PCA9685_PRESCALE 0xFE     /**< Prescaler for PWM output frequency */
#define PCA9685_LED0_ON_L 0x06  /**< LED0 on tick, low byte*/
#define PCA9685_LED0_ON_H 0x07  /**< LED0 on tick, high byte*/
#define PCA9685_LED0_OFF_L 0x08 /**< LED0 off tick, low byte */
#define PCA9685_LED0_OFF_H 0x09 /**< LED0 off tick, high byte */
#define PCA9685_ALLLED_ON_L 0xFA  /**< load all the LEDn_ON registers, low */
// MODE1 bits
#define MODE1_ALLCAL 0x01  /**< respond to LED All Call I2C-bus address */
#define MODE1_SUB3 0x02    /**< respond to I2C-bus subaddress 3 */
#define MODE1_SUB2 0x04    /**< respond to I2C-bus subaddress 2 */
#define MODE1_SUB1 0x08    /**< respond to I2C-bus subaddress 1 */
#define MODE1_SLEEP 0x10   /**< Low power mode. Oscillator off */
#define MODE1_AI 0x20      /**< Auto-Increment enabled */
#define MODE1_EXTCLK 0x40  /**< Use EXTCLK pin clock */
#define MODE1_RESTART 0x80 /**< Restart enabled */
// MODE2 bits
#define MODE2_OUTDRV 0x04 /**< totem pole structure vs open-drain */

// Useful bitwise operators
#define PCA9685_SET_BIT_MASK(BYTE, MASK)      ((BYTE) |= (uint8_t)(MASK))
#define PCA9685_CLEAR_BIT_MASK(BYTE, MASK)    ((BYTE) &= (uint8_t)(~(uint8_t)(MASK)))
#define PCA9685_READ_BIT_MASK(BYTE, MASK)     ((BYTE) & (uint8_t)(MASK)) 

bool is_sleeping(bool *sleeping){
	bool success = true;
	// Read the current state of the mode 1 register.
	uint8_t mode1_reg;
	success &= rc_i2c_read_byte(EXTERNAL_I2C_BUS, PCA9685_MODE1, &mode1_reg)!=-1;
	// Check if the sleeping bit is set.
	*sleeping = mode1_reg & MODE1_SLEEP ;
    return success;
}

bool set_pwm_frequency(){
    bool success = true;
    uint8_t prescaler = 0x03; // minimum prescaler
    //bool already_sleeping; // flag to check if the board is sleeping
    success &= rc_i2c_write_byte(EXTERNAL_I2C_BUS, PCA9685_PRESCALE, prescaler)==0;
    return success;
}

bool wakeup(){
    bool success = true;
    uint8_t mode1_reg;
    success&=rc_i2c_read_byte(EXTERNAL_I2C_BUS, PCA9685_MODE1, &mode1_reg)!=-1;
    bool restart_required = mode1_reg & MODE1_RESTART;// PCA9685_REGISTER_MODE1_RESTART);     
    // Clear the restart bit for now and clear the sleep bit.
    mode1_reg &= (uint8_t)(~(uint8_t)MODE1_RESTART);
    mode1_reg &= (uint8_t)(~(uint8_t)MODE1_SLEEP);
    success &= rc_i2c_write_byte(EXTERNAL_I2C_BUS, PCA9685_MODE1, mode1_reg)==0;
    rc_usleep(1000);
    if(restart_required){
        mode1_reg |= (uint8_t)(MODE1_RESTART);
        success &= rc_i2c_write_byte(EXTERNAL_I2C_BUS,PCA9685_MODE1, mode1_reg)==0;
    }
    return success;
}


void init(){
  bool sucess=true;
  const uint8_t mode1_reg_default_value = 0b00110000u,
                mode2_reg_default_value = 0b00000100u;
  // set external i2c bus to board address
  sucess &=rc_i2c_set_device_address(EXTERNAL_I2C_BUS, PCA9685_I2C_ADDRESS)!=-1;
  // restore mode1,2 registers to default
  sucess &=rc_i2c_write_byte(EXTERNAL_I2C_BUS, PCA9685_MODE1, mode1_reg_default_value)==0;
  sucess &=rc_i2c_write_byte(EXTERNAL_I2C_BUS, PCA9685_MODE2, mode2_reg_default_value)==0;
  rc_usleep(1000);
  // Turn all channels off to begin with.
  uint8_t data[4] = { 0x00, 0x00, 0x00, 0x10 };
  sucess &=rc_i2c_write_bytes(EXTERNAL_I2C_BUS,PCA9685_ALLLED_ON_L,4,data )==0;
  // configure pwm base frequency
  sucess &= set_pwm_frequency();
  // wakeup board 
  sucess &= wakeup();
  //return sucess;
  printf("%s\n", sucess?"init correct":"init nope");
  
}



bool set_channel_pwm_times(uint8_t channel, uint16_t on_time, uint16_t off_time){
    bool success = true;
    success &= rc_i2c_set_device_address(EXTERNAL_I2C_BUS, PCA9685_I2C_ADDRESS)!=-1;  
	assert(channel >= 0);
	assert(channel < 16);

	assert(on_time >= 0);
	assert(on_time <= 4096);

	assert(off_time >= 0);
	assert(off_time <= 4096);
    //printf("on_time: %d \t off_time: %d\n",on_time,off_time);
	uint8_t data[5] = {PCA9685_LED0_ON_L + channel * 4, on_time, on_time >> 8, off_time, off_time >> 8 };
	rc_i2c_send_bytes(EXTERNAL_I2C_BUS, 5, data);
	//uint8_t data[5] = {PCA9685_LED0_ON_L + channel * 4, on_time, on_time >> 8, off_time, off_time >> 8 };
	/*
	success &=rc_i2c_write_byte(EXTERNAL_I2C_BUS, PCA9685_LED0_ON_L + 4 * channel, data[0])!=-1;
    success &=rc_i2c_write_byte(EXTERNAL_I2C_BUS, PCA9685_LED0_ON_H + 4 * channel, data[1])!=-1;
	*/
	
	//for(uint8_t i=0; i<5;i++) printf("data_%d: 0x%02x\t",i,data[i]);
	//printf("\n");
	
	//success &=rc_i2c_write_byte(EXTERNAL_I2C_BUS, PCA9685_LED0_OFF_L, data[2])!=-1;
    //success &=rc_i2c_write_byte(EXTERNAL_I2C_BUS, PCA9685_LED0_OFF_H, data[3])!=-1;    
    
//	rc_i2c_send_byte(EXTERNAL_I2C_BUS, PCA9685_LED0_ON_L + channel * 4);
  //  return rc_i2c_send_bytes(EXTERNAL_I2C_BUS, 4, data)!=-1;
    return success;
}

bool set_channel_duty_cycle(uint8_t channel, float duty_cycle){
	assert(duty_cycle >= 0.0);
	assert(duty_cycle <= 1.0);
	if (duty_cycle == 0.0) {
		return set_channel_pwm_times(channel, 0, 4096); // Special value for always off
	} else if (duty_cycle == 1.0) {
		return set_channel_pwm_times(channel, 4096, 0); // Special value for always on
	} else {
	uint16_t required_on_time= (uint16_t)roundf(4095 * duty_cycle);
		// Offset on and off times depending on channel to minimise current spikes.
	uint16_t on_time = (channel == 0) ? 0 : (channel * 256) - 1; // 0
	uint16_t off_time = (on_time + required_on_time) & 0x0fffu;
    printf("on_time: %d \t off_time: %d\n",on_time,off_time);
	return set_channel_pwm_times(channel, on_time, off_time);
	}
}

int main(){
// ========= Signal handler
    if(rc_kill_existing_process(2.0)<-2) return -1; 
    // Make sure no other process is running
    if(rc_enable_signal_handler()==-1){
                logger::error("ERROR: failed to start signal handler");
                return EXIT_FAILURE;
    }  
    signal(SIGINT, __signal_handler);
    rc_make_pid_file();
    running = 1;
// ========= Initialize I^2C bus  
    if(rc_i2c_init(EXTERNAL_I2C_BUS, PCA9685_I2C_ADDRESS)!=0){
        logger::error("Failed to initialize I2C external bus");
        return EXIT_FAILURE;
    }
// =========    
    /*
    uint8_t mode1_reg, mode2_reg;
    rc_i2c_read_byte(EXTERNAL_I2C_BUS,PCA9685_MODE1,&mode1_reg);
    rc_i2c_read_byte(EXTERNAL_I2C_BUS,PCA9685_MODE2,&mode2_reg);
    printf("MODE1: 0x%02x\tMODE2: 0x%02x\n",mode1_reg,mode2_reg);
    */

    init();
    rc_i2c_read_byte(EXTERNAL_I2C_BUS,PCA9685_MODE1,&mode1_reg);
    rc_i2c_read_byte(EXTERNAL_I2C_BUS,PCA9685_MODE2,&mode2_reg);
    printf("MODE1: 0x%02x\tMODE2: 0x%02x\n",mode1_reg,mode2_reg); 
    
    uint8_t prescaler;
    rc_i2c_read_byte(EXTERNAL_I2C_BUS,PCA9685_PRESCALE,&prescaler);
    printf("Prescaler 0x%02x\n",prescaler); 
    
    
    //set_channel_duty_cycle(1,0.25);
    //printf("Wrote PWM? %s \n", set_channel_duty_cycle(1,0.25)?"Yes":"no");
    uint8_t on_L, on_H, off_L, off_H;
    float duty_cycle = 0.8;
    uint8_t channel = 0;
    uint16_t required_on_time= (uint16_t)roundf(4095 * duty_cycle);
    uint16_t on_time = 0;//(channel == 0) ? 0 : (channel * 256) - 1;
	uint16_t off_time = 4095;//(on_time + required_on_time) & 0xfffu;
   // uint8_t data[5] = {PCA9685_LED0_ON_L + channel * 4, on_time, on_time >> 8, off_time, off_time >> 8 };
    //uint8_t data[5] = {PCA9685_LED0_ON_L + channel * 4, 0x65, 0x0E, 0xCB, 0x0C };
    
    uint8_t data[5] = {PCA9685_LED0_ON_L + channel * 4, on_time, on_time >> 8, off_time, off_time >> 8 };
    rc_i2c_read_byte(EXTERNAL_I2C_BUS,PCA9685_LED0_ON_L+ channel * 4,&on_L);
    rc_i2c_read_byte(EXTERNAL_I2C_BUS,PCA9685_LED0_ON_H+ channel * 4,&on_H);
    printf("_ON_L: 0x%02x\tON_L: 0x%02x\t ON:%d\n",on_L,on_H, on_H<<8 | on_L); 
    rc_i2c_read_byte(EXTERNAL_I2C_BUS,PCA9685_LED0_OFF_L+ channel * 4,&off_L);
    rc_i2c_read_byte(EXTERNAL_I2C_BUS,PCA9685_LED0_OFF_H+ channel * 4,&off_H);
    printf("_Off_L: 0x%02x\tOff_L: 0x%02x\t Off:%d\n",off_L,off_H, off_H<<8 | off_L); 
    
    //PCA9685_LED0_ON_L
    /*rc_i2c_write_byte(EXTERNAL_I2C_BUS, PCA9685_LED0_ON_L, 0); 
    rc_i2c_write_byte(EXTERNAL_I2C_BUS, PCA9685_LED0_ON_H,   0xFF);  
    rc_i2c_write_byte(EXTERNAL_I2C_BUS, PCA9685_LED0_OFF_L, 0); 
    rc_i2c_write_byte(EXTERNAL_I2C_BUS, PCA9685_LED0_OFF_H,   0); */
    
    //uint8_t data[4] = { 0x00, 0x00, 0x00, 0x10 };
    
    //uint8_t data[5] = {PCA9685_LED0_ON_L + channel * 4, 0x00, 0x00, off_time, off_time >> 8 };
    //uint8_t data[5] = {PCA9685_LED0_ON_L + channel * 4, on_time, on_time >> 8, 0x00, 0x00 };
    //
    /*
    rc_i2c_write_byte(EXTERNAL_I2C_BUS, PCA9685_LED0_ON_L, data[0]); 
    rc_i2c_write_byte(EXTERNAL_I2C_BUS, PCA9685_LED0_ON_H, data[1]);  
    rc_i2c_write_byte(EXTERNAL_I2C_BUS, PCA9685_LED0_OFF_L,data[2]); 
    rc_i2c_write_byte(EXTERNAL_I2C_BUS, PCA9685_LED0_OFF_H,data[3]);
    */
   // rc_i2c_send_byte(EXTERNAL_I2C_BUS, PCA9685_LED0_ON_L + channel * 4);
    //rc_i2c_send_bytes(EXTERNAL_I2C_BUS, 5, data);
    int dc = 0;
     set_channel_duty_cycle(0,.9);
    while(running){
       // dc = (dc+10)%101;
        //set_channel_duty_cycle(0,dc*0.01);
        
        rc_usleep(US_SECOND);
    }
    set_channel_duty_cycle(0,0);
    //
    //for(uint8_t i=0; i<5;i++) printf("data_%d: %02x\t",i,data[i]);
	//printf("\n");
    //
    rc_i2c_read_byte(EXTERNAL_I2C_BUS,PCA9685_LED0_ON_L+channel * 4,&on_L);
    rc_i2c_read_byte(EXTERNAL_I2C_BUS,PCA9685_LED0_ON_H+channel * 4,&on_H);
    printf("_ON_L: 0x%02x\t _ON_L: 0x%02x\t ON:%d\n",on_L,on_H, on_H<<8 | on_L); 
    rc_i2c_read_byte(EXTERNAL_I2C_BUS,PCA9685_LED0_OFF_L+channel * 4,&off_L);
    rc_i2c_read_byte(EXTERNAL_I2C_BUS,PCA9685_LED0_OFF_H+channel * 4,&off_H);
    printf("_Off_L: 0x%02x\t _Off_L: 0x%02x\t Off:%d\n",off_L,off_H, off_H<<8 | off_L); 
    
    rc_i2c_close(EXTERNAL_I2C_BUS);
    rc_remove_pid_file();    
    
    return 0;
}