#include<iostream>
#include <signal.h>
#include <fstream>

#include<robotcontrol.h>
#include<HardwareBeagleBoneBlue.hpp>
static int running = 0;

// interrupt handler to catch ctrl-c
static void __signal_handler(__attribute__ ((unused)) int dummy)
{
        running=0;
        rc_set_state(EXITING);
        return;
}

int main(){
    if(rc_kill_existing_process(2.0)<-2) return -1; // Make sure no other process is running
    if(rc_enable_signal_handler()==-1){
                std::cerr<<"ERROR: failed to start signal handler\n";
                return EXIT_FAILURE;
    }  
    signal(SIGINT, __signal_handler);
    rc_make_pid_file();
    running =1;
    if(rc_motor_init_freq(20000)==-1){
        std::cerr<<"ERROR: failed to initialize motor drivers\n";
        return EXIT_FAILURE;       
    }   
    rc_motor_standby(1); // start with motors in standby
    
    
    std::ofstream current_readings;
    current_readings.open("data.csv");
    
    BeagleBoneBlueCurrentSensor ina260;
    

    rc_set_state(RUNNING);
    double motor_current=0.0;
    while(rc_get_state()!=EXITING){
        rc_motor_set(3,0.25);
        ina260.get_left_motor_current(motor_current);
        current_readings << motor_current << '\n';      
        rc_usleep(500000);
    }

   current_readings.close();
   rc_remove_pid_file();    
   rc_motor_cleanup();


    return 0;
}