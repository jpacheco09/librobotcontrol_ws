#include<iostream>
#include<signal.h>
#include<chrono>

#include<rc/start_stop.h>
#include<rc/encoder_eqep.h>
#include<rc/time.h>
#include<rc/i2c.h>

#include<pca9685.hpp>
#include<ina260.hpp>
/*
Siento que he procrastinado mucho hacer cualquier prueba con mi plataforma por 
miedo a que algo falle, o bien, que no funcione a la primera y no ser capaz de 
i) entender, ii) arreglarlo. Sin dudas necesito que me guien, o bien, dejar de 
ser tan qlombrga y ponerme a darle, sin miedo a dañar algo. 
*/
//------------------------------------------------------------------------------  
static int running = 0;
// interrupt handler ctrl-c
static void __signal_handler(__attribute__ ((unused)) int dummy){
    running = 0;
    rc_set_state(EXITING);
}
//------------------------------------------------------------------------------  
#define EQEP_RIGHT_MOTOR 1
#define EQEP_LEFT_MOTOR  2
#define US 1000000
#define loop_freq 5000
//------------------------------------------------------------------------------
/*
void read_current(double &current){
    uint16_t _word_rx_buffer;
    int16_t _sign_conversion_buffer;
   // rc_i2c_set_device_address(ina260::BUS_INFO::EXTERNAL_I2C_BUS, 
    //                          ina260::BUS_INFO::ina260_LEFT_MOTOR_ADDRESS);    
    rc_i2c_read_word(ina260::BUS_INFO::EXTERNAL_I2C_BUS,
                     ina260::REGISTER_ADDRESS::CURRENT,
                     &_word_rx_buffer);
    _sign_conversion_buffer = (int16_t) _word_rx_buffer;
    current = ina260::K_current_conversion * (double)(_sign_conversion_buffer);
}
*/
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
    running = 1;
//The process is properly running, now let us initialize the board pheriferials
//------------------------------------------------------------------------------  
// default i2c external modue
    if(rc_i2c_init(EXTERNAL_I2C_BUS,pca9685::default_address)!=0){
        std::cerr<<"[E] Failed to initialize I2C bus/no PWM module attached\n";
        return EXIT_FAILURE;
    }
// initialize and configure left current sensor module
    if(ina260::init(ina260::K_DEFAULT_LEFT_MOTOR_ADDRESS_)!=0){
        std::cerr<<"[E] Failed to initialize left current sensor\n";
        return EXIT_FAILURE;                
    }
// configure sensor to default
    const uint16_t K_test_config = 0x6000 | // default preffix
    ina260::CONFIGURATION_REGISTER::AVG_MOD_4S | // average each 1 samples
    ina260::CONFIGURATION_REGISTER::ISHCT_588us|//current 1.1ms convertion time
    ina260::CONFIGURATION_REGISTER::ISHCT_1100us | 
    ina260::CONFIGURATION_REGISTER::MODE_Co_I; // continous current read
    ina260::configure_sensor(ina260::K_DEFAULT_LEFT_MOTOR_ADDRESS_, 
                             K_test_config);
    std::cout<<"Sensor initialized\n";
    rc_usleep(50000); // wait for measurements to stabilize                             
//------------------------------------------------------------------------------  
// open file to write
    FILE* pFile;                            
    pFile = fopen("current_cal_test1.csv", "w");
// set timming     
   std::chrono::system_clock::time_point last_time = 
                        std::chrono::high_resolution_clock::now(), current_time;
   std::chrono::duration<double> elapsed_time;    
//------------------------------------------------------------------------------  
// main stuff
// we shall take 1000 samples to test our settings
double current=0.0;
    for(int i=0; i<= 1000; i++){ 
        current_time = std::chrono::high_resolution_clock::now();   
        current=ina260::read_current(); // read default 0x40 address     
        elapsed_time = current_time - last_time;
        //std::cout<<elapsed_time.count()<<std::endl;
        fprintf(pFile, "%f,%f\n",elapsed_time.count(),current);
        last_time = current_time;
        rc_usleep(1000); // 1 kHz sampling time base
   }
//------------------------------------------------------------------------------
// close the file
    fclose(pFile);
// The work is done, now clear and close your periferics
    rc_i2c_close(1);    
    rc_remove_pid_file(); // make sure the process is properly ended
    std::cout<<"Processes closed succesfully\n";
    return 0;
}