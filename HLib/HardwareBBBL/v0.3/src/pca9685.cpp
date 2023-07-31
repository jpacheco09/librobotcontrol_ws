#include<rc/i2c.h>
#include<rc/time.h>

#include<iostream>
#include <math.h>

#include<pca9685.hpp>
#ifndef __cplusplus
#include <stdbool.h>
#endif

int pca9685::init(uint8_t address){
    bool sucess=true;    
    // set external i2c bus to board address
    sucess &= rc_i2c_set_device_address(EXTERNAL_I2C_BUS,address)!=-1;
    // restore mode1,2 registers to default
    sucess &=rc_i2c_write_byte(EXTERNAL_I2C_BUS, pca9685::REGISTER_ADDRESS::MODE1, pca9685::MODE1::DEFAULT)!=-1;
    sucess &=rc_i2c_write_byte(EXTERNAL_I2C_BUS, pca9685::REGISTER_ADDRESS::MODE2, pca9685::MODE2::DEFAUL)!=-1;
    // allow oscillator to stabilize
    rc_usleep(1000);
    // Turn all channels off to begin with.    
    uint8_t data[4] = { 0x00, 0x00, 0x00, 0x10 };
    sucess &=rc_i2c_write_bytes(EXTERNAL_I2C_BUS,pca9685::REGISTER_ADDRESS::ALLLED_ON_L,4,data)!=-1;
    // configure pwm base frequency
    sucess &= set_pwm_frequency();
    // wakeup board 
     sucess &= wakeup();
    return sucess;
    //printf("%s\n", sucess?"init correct":"init nope");    
}

bool pca9685::set_pwm_frequency(void){
    bool success = true;
    uint8_t prescaler = 0x03; // minimum prescaler
    //bool already_sleeping; // flag to check if the board is sleeping
    success &= rc_i2c_write_byte(EXTERNAL_I2C_BUS, pca9685::REGISTER_ADDRESS::PRESCALE, prescaler)==0;
    return success;
}

bool pca9685::wakeup(void){
    bool success = true;
    uint8_t mode1_reg;
    success&=rc_i2c_read_byte(EXTERNAL_I2C_BUS, pca9685::REGISTER_ADDRESS::MODE1, &mode1_reg)!=-1;
    bool restart_required = mode1_reg & pca9685::MODE1::RESTART;// PCA9685_REGISTER_MODE1_RESTART);     
    // Clear the restart bit for now and clear the sleep bit.
    mode1_reg &= (uint8_t)(~(uint8_t)pca9685::MODE1::RESTART);
    mode1_reg &= (uint8_t)(~(uint8_t)pca9685::MODE1::SLEEP);
    success &= rc_i2c_write_byte(EXTERNAL_I2C_BUS, pca9685::REGISTER_ADDRESS::MODE1, mode1_reg)==0;
    // allow oscillator to stabilize
    rc_usleep(1000);
    if(restart_required){
        mode1_reg |= (uint8_t)(pca9685::MODE1::RESTART);
        success &= rc_i2c_write_byte(EXTERNAL_I2C_BUS,pca9685::REGISTER_ADDRESS::MODE1, mode1_reg)==0;
    }
    return success;
}

int pca9685::set_channel_duty_cycle(uint8_t address,uint8_t channel, float duty_cycle){
    //sanity checks
    if(duty_cycle < 0.0 ||  duty_cycle > 1.0){
        std::cerr<<"[Error](pca9685::set_channel_duty_cycle) Invalid duty_cycle\n";
        return -1;
    }
    if (duty_cycle == 0.0) {
		return set_channel_pwm_times(address,channel, 0, 4096); // Special value for always off
	} else if (duty_cycle == 1.0) {
		return set_channel_pwm_times(address,channel, 4096, 0); // Special value for always on
	} else {
	uint16_t required_on_time= (uint16_t)roundf(4095 * duty_cycle);
		// Offset on and off times depending on channel to minimise current spikes.
	uint16_t on_time = (channel == 0) ? 0 : (channel * 256) - 1;
	uint16_t off_time = (on_time + required_on_time) & 0xfffu;
    //printf("on_time: %d \t off_time: %d\n",on_time,off_time);
	return set_channel_pwm_times(address,channel, on_time, off_time);
	}
}

int pca9685::set_channel_pwm_times(uint8_t address, uint8_t channel, uint16_t on_time, uint16_t off_time){
    bool success = true;
    // sanity checks 
    if(channel < 0 ||  channel > 15){
        std::cerr<<"[Error](pca9685::set_channel_duty_cycle) Invalid channel\n";
        return -1;
    }
    if(on_time < 0 ||  on_time > 4096){
        std::cerr<<"[Error](pca9685::set_channel_pwm_times) Invalid on_time\n";
        return -1;
    }    
    if(off_time < 0 ||  off_time > 4096){
        std::cerr<<"[Error](pca9685::set_channel_pwm_times) Invalid off_time\n";
        return -1;
    } 
    //printf("on_time: %d \t off_time: %d\n",on_time,off_time);
    success &= rc_i2c_set_device_address(EXTERNAL_I2C_BUS, pca9685::default_address)!=-1;
    uint8_t data[5] = {static_cast<uint8_t>(pca9685::LED0_ON_L+ channel * 4),
                       static_cast<uint8_t>(on_time),
                       static_cast<uint8_t>(on_time >> 8),
                       static_cast<uint8_t>(off_time),
                       static_cast<uint8_t> (off_time >> 8) };
    success &= rc_i2c_send_bytes(EXTERNAL_I2C_BUS, 5, data) !=-1;
    //for(uint8_t i=0; i<5;i++) printf("data_%d: 0x%02x\t",i,data[i]);
	//printf("\n");
    return success;
}    