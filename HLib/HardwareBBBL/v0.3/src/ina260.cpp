#include<rc/i2c.h>
#include<rc/time.h>

#include<iostream>

#include<ina260.hpp>
#ifndef __cplusplus
#include <stdbool.h>
#endif

//using namespace ina260;

int ina260::init(uint8_t address){
    bool success = true;
    // sanity checks 
    if(address < 0x40 || address > 0x45 ){
        std::cerr<<"[Error](ina260::init) Invalid address\n";
        return -1 ;
    }
    /**
     * @todo Check device signature and chip name to test if it's an INA260.
     * 
     */

    if(rc_i2c_set_device_address(EXTERNAL_I2C_BUS, address)==-1){
        std::cerr<<"[Error](ina260::init) Unable to reach sensor \n";  
        return -1;      
    }
    success &= ina260::configure_sensor(address) != -1;
    return success - 1;
}

int ina260::configure_sensor(uint8_t address, uint16_t configuration){
    bool success = true;
    if(rc_i2c_write_word(EXTERNAL_I2C_BUS, ina260::REGISTER_ADDRESS::CONFIGURATION, configuration) == -1){
        std::cerr<<"[Error](ina260::configure_sensor) Unable to configure \n";
        return -1;
    }
    rc_usleep(5000);
    return success - 1;
}

int ina260::configure_sensor(uint16_t configuration){
    return configure_sensor(0x40, configuration);
}

int ina260::configure_sensor(void){
    return configure_sensor(ina260::INA260_CURRENT_READING_CONFIGURATION_N);
}

double ina260::read_current(uint8_t address){
    uint16_t _word_rx_buffer;
    // Set sensor address
    if(rc_i2c_set_device_address(EXTERNAL_I2C_BUS, address)==-1){
        std::cerr<<"[Error](ina260::read_current) Unable to reach sensor \n";  
        return -1;      
    }
    // read current register
    if(rc_i2c_read_word(EXTERNAL_I2C_BUS, ina260::REGISTER_ADDRESS::CURRENT, &_word_rx_buffer)==-1){
        std::cerr<<"[Error](ina260::read_current) Unable to read current \n";  
        return -1;             
    }
    // current conversion

    return ina260::K_current_conversion * ((int16_t)_word_rx_buffer);

}

double ina260::read_current(void){
    return read_current(0x40);
}

double ina260::read_voltage(uint8_t address){
    uint16_t _word_rx_buffer;
    // Set sensor address
    if(rc_i2c_set_device_address(EXTERNAL_I2C_BUS, address)==-1){
        std::cerr<<"[Error](ina260::read_voltage) Unable to reach sensor \n";  
        return -1;      
    }
    // read current register
    if(rc_i2c_read_word(EXTERNAL_I2C_BUS, ina260::REGISTER_ADDRESS::BUS_VOLTAGE, &_word_rx_buffer)==-1){
        std::cerr<<"[Error](ina260::read_voltage) Unable to read current \n";  
        return -1;             
    }
    // current conversion

    return ina260::K_voltage_conversion * ((int16_t)_word_rx_buffer);

}

double ina260::read_voltage(void){
    return read_voltage(0x40);
}