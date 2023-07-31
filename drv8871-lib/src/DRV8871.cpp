/**
 * @file DRV8871.cpp
 * @author Jesus Pacheco
 * @date 2022
 */

#include <stdio.h>
#include <DRV8871.hpp>
#include <rc/pwm.h>
#include <rc/pinmux.h>

int pinmux_pwms_to_uart_ports(void){
    
    return 0;
}


int motor_init_freq(const int &pwm_frequency_hz){
    
    pinmux_pwms_to_uart_ports();
    // PWM0 is at UART GPS port
    //  
    configure_pwm_channels();





    return 1;
}