#include<iostream>
#include<robotcontrol.h>
#include <rc/pthread.h>

#include <HardwareBeagleBoneBlue.hpp>

#define US_SECOND 1000000
#define PRINT_FREQ 100

void* _print_state_loop(){
    rc_state_t current_state, next_state;
    current_state = rc_get_state(); // get current state at the time that the thread is called
    while(current_state!=EXITING){
        if(current_state == PAUSED)
            printf("")

        rc_usleep(US_SECOND/PRINT_FREQ); // send thread to sleep when printing is done
    }



    
}


int main(){
    if(rc_kill_existing_process(2.0)<-2) return -1;
    if(rc_enable_signal_handler()==-1){
                fprintf(stderr,"ERROR: failed to start signal handler\n");
                return EXIT_FAILURE;
    }  
    rc_make_pid_file();
    
    pthread_t battery_thread = 0;
    pthread_t printf_thread = 0;   

    bool print_thread_init = false;

    //print_thread_init = 

    return 0;
}