#include<robotcontrol.h>
#include<iostream>

/**
 * @brief Drive H-Bridged DC MOTOR 
 * @param
 *         ENA/DIRA [
 *         
 * @todo:
 *      -init: pinmux PWM pins to corresponding outputs
 *             set default frequency
 *             
 *       -cleanup:        
 *       -break: routine to execute before power off
 * 
 * @return int 
 */

int main(){
    if(rc_kill_existing_process(2.0)<-2) return -1;
    if(rc_enable_signal_handler()==-1){
                std::cerr<<"ERROR: failed to start signal handler\n";
                return EXIT_FAILURE;
    }  

    if(rc_pinmux_set(GPS_HEADER_PIN_3 , rc_pinmux_mode_t::PINMUX_PWM)==-1){ // GPS UART2 RX ehrpwm0B
                std::cerr<<"ERROR: failed to pinmux GP0_PIN_3 to PWM\n";
                return EXIT_FAILURE;
    }    
    if(rc_pinmux_set(GPS_HEADER_PIN_4, rc_pinmux_mode_t::PINMUX_PWM)==-1){ // GPS UART2 TX ehrpwm0A
                std::cerr<<"ERROR: failed to pinmux GP0_PIN_4 to PWM\n";
                return EXIT_FAILURE;
    }     
    if(rc_pwm_init(0, 10000)==-1){ //
                std::cerr<<"ERROR: failed to pinmux GP0_PIN_4 to PWM\n";
                return EXIT_FAILURE;
    }
    rc_make_pid_file();
    
    rc_set_state(RUNNING);
    while(rc_get_state()!=EXITING){
        rc_pwm_set_duty(0, 'A', 0.5 );
        rc_pwm_set_duty(0, 'B', 0.25 );    
        
        rc_usleep(100000);
    }

    
    
    rc_pwm_cleanup(0);
    rc_pinmux_set_default();
    rc_remove_pid_file();    
    


    return 0;
}