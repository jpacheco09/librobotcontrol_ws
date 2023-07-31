#include<iostream>
#include<signal.h>

#include<robotcontrol.h>

#define US_SECOND 1000000
static int running = 0;
// interrupt handler ctrl-c
static void __signal_handler(__attribute__ ((unused)) int dummy){
    running = 0;
    rc_set_state(EXITING);
}


int main(){
// ========= Signal handler
    if(rc_kill_existing_process(2.0)<-2) return -1; // Make sure no other process is running
    if(rc_enable_signal_handler()==-1){
                std::cerr<<"ERROR: failed to start signal handler\n";
                return EXIT_FAILURE;
    }  
    signal(SIGINT, __signal_handler);
    rc_make_pid_file();
// ============Init servo module =================
    // initialize PRU
    if(rc_servo_init()) return -1;
    running =1;    
    int width = 1000;
    const int main_loop_freq = 100;//hz;
    while(running){

        printf("Set pulse width ranging from (1000,2000)[us]: \n");
        scanf("%d",&width);
        rc_servo_send_pulse_us(0,width);
        rc_usleep(floor(US_SECOND/main_loop_freq));

    }

    rc_servo_cleanup();
    rc_remove_pid_file();
    return 0;
}