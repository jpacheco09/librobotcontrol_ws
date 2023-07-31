#include <robotcontrol.h>
#include <iostream>
#include <chrono>
bool init = false;
void write_motors(std::chrono::system_clock::time_point &last_time ){
    std::chrono::system_clock::time_point current_time = std::chrono::high_resolution_clock::now(); 
    std::chrono::duration<double> elapsed_time = current_time - last_time; // obtain Delta_t       
    std::cout<<elapsed_time.count()<<std::endl;
    if(init == false){
        rc_motor_set(1,0.1);
        rc_motor_set(2,0.2);
        init = true;
    }else rc_usleep(100000);
    
    last_time = current_time;
}

int main(){

    if(rc_kill_existing_process(2.0)<-2) return EXIT_FAILURE;
    if(rc_enable_signal_handler()==-1) return EXIT_FAILURE;
    rc_motor_init_freq(20000);




    rc_make_pid_file();

    rc_set_state(RUNNING);
    std::chrono::system_clock::time_point current_time = std::chrono::high_resolution_clock::now();
    
    while(rc_get_state()!=EXITING) write_motors(current_time);

    rc_remove_pid_file();
    rc_motor_cleanup();    
    return 0;
}
/*
int main()
{
        // make sure another instance isn't running
        // if return value is -3 then a background process is running with
        // higher privaledges and we couldn't kill it, in which case we should
        // not continue or there may be hardware conflicts. If it returned -4
        // then there was an invalid argument that needs to be fixed.
        if(rc_kill_existing_process(2.0)<-2) return -1;
        // start signal handler so we can exit cleanly
        if(rc_enable_signal_handler()==-1){
                fprintf(stderr,"ERROR: failed to start signal handler\n");
                return -1;
        }
        // initialize pause button
        if(rc_button_init(RC_BTN_PIN_PAUSE, RC_BTN_POLARITY_NORM_HIGH,
                                                RC_BTN_DEBOUNCE_DEFAULT_US)){
                fprintf(stderr,"ERROR: failed to initialize pause button\n");
                return -1;
        }
        // Assign functions to be called when button events occur
        rc_button_set_callbacks(RC_BTN_PIN_PAUSE,on_pause_press,on_pause_release);
        // make PID file to indicate your project is running
        // due to the check made on the call to rc_kill_existing_process() above
        // we can be fairly confident there is no PID file already and we can
        // make our own safely.
        rc_make_pid_file();
        printf("\nPress and release pause button to turn green LED on and off\n");
        printf("hold pause button down for 2 seconds to exit\n");
        // Keep looping until state changes to EXITING
        rc_set_state(RUNNING);
        while(rc_get_state()!=EXITING){
                // do things based on the state
                if(rc_get_state()==RUNNING){
                        rc_led_set(RC_LED_GREEN, 1);
                        rc_led_set(RC_LED_RED, 0);
                }
                else{
                        rc_led_set(RC_LED_GREEN, 0);
                        rc_led_set(RC_LED_RED, 1);
                }
                // always sleep at some point
                rc_usleep(100000);
        }
        // turn off LEDs and close file descriptors
        rc_led_set(RC_LED_GREEN, 0);
        rc_led_set(RC_LED_RED, 0);
        rc_led_cleanup();
        rc_button_cleanup();    // stop button handlers
        rc_remove_pid_file();   // remove pid file LAST
        return 0;
}*/