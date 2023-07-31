#include<iostream>
#include<signal.h>

#include<robotcontrol.h>
#include<pca9685.hpp>
#include<ina260.hpp>

#define US_SECOND 1000000

static int running = 0;
// interrupt handler ctrl-c
static void __signal_handler(__attribute__ ((unused)) int dummy){
    running = 0;
    rc_set_state(EXITING);
}
//------------------------------------------------------------------------------  
int main(){
// first of all, we shall asign a singal handler to this process
    if(rc_kill_existing_process(2.0)<-2) return -1;//no other process is running
    if(rc_enable_signal_handler()==-1){
        std::cerr<<"[E] Failed to enable signal handler\n";
        return EXIT_FAILURE;
    }//signal handler
    signal(SIGINT, __signal_handler);
    rc_make_pid_file();
//The process is properly running, now let us initialize the boars pheriferials
//------------------------------------------------------------------------------  
// default i2c external modue
    if(rc_i2c_init(EXTERNAL_I2C_BUS,pca9685::default_address)!=0){
        std::cerr<<"[E] Failed to initialize I2C bus/no PWM module attached\n";
        return EXIT_FAILURE;
    }
// initialize pwm module
    if(pca9685::init(pca9685::default_address)!=true){
        std::cerr<<"[E] Failed to initialize PWM module\n";
        return EXIT_FAILURE;
    }
// initialize and configure left current sensor module
    if(ina260::init(ina260::K_DEFAULT_LEFT_MOTOR_ADDRESS_)!=0){
        std::cerr<<"[E] Failed to initialize left current sensor\n";
        return EXIT_FAILURE;                
    }
    ina260::configure_sensor(ina260::K_DEFAULT_LEFT_MOTOR_ADDRESS_, 
                             ina260::INA260_CURRENT_READING_CONFIGURATION_H);
// initialize and configure right current sensor module
    if(ina260::init(ina260::K_DEFAULT_RIGHT_MOTOR_ADDRESS_)!=0){
        std::cerr<<"[E] Failed to initialize right current sensor\n";
        return EXIT_FAILURE;                
    }   
    ina260::configure_sensor(ina260::K_DEFAULT_RIGHT_MOTOR_ADDRESS_, 
                             ina260::INA260_CURRENT_READING_CONFIGURATION_H); 
//------------------------------------------------------------------------------  
// if all modules were properly configured, let us show a success message
    std::cout<<"All modules were properly configured\n";
//------------------------------------------------------------------------------
    rc_i2c_close(EXTERNAL_I2C_BUS);
    rc_remove_pid_file(); // make sure the process is properly ended
    return 0;
}