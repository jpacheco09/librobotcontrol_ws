#include<iostream>
#include<signal.h>

#include<rc/start_stop.h>
#include<rc/encoder_eqep.h>
#include<rc/time.h>
#include<rc/i2c.h>

#include<pca9685.hpp>
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
// initialize pwm module
    if(pca9685::init(pca9685::default_address)!=true){
        std::cerr<<"[E] Failed to initialize PWM module\n";
        return EXIT_FAILURE;
    }
//------------------------------------------------------------------------------  
// now let us initialize the eqep module
    if(rc_encoder_eqep_init()!=0){
        std::cout<<"[E] Failed to initialize EQEP module\n";
        return EXIT_FAILURE;
    }
//------------------------------------------------------------------------------
pca9685::set_channel_duty_cycle(0x42, 0, 0.25);
pca9685::set_channel_duty_cycle(0x42, 1, 0.0);
//------------------------------------------------------------------------------
// For illustration pourposes, i'll loop around to show both, angular velocity 
// and the angular position of the wheels
    long long encoder_count[2] = {0,0};
    float angular_position[2] = {0.0,0.0};
    const int8_t K_r_wheel_polarity=1,K_l_wheel_polarity=1, K_motor_gearbox = 131,    
            K_encoder_resolution = 64;
    const float K_wheel_radius =  65e-3/2; // 63mm wheels?
    angular_position[0] = encoder_count[0] ; // rightWheelAngle
    while(running){
        encoder_count[0] = rc_encoder_eqep_read(EQEP_RIGHT_MOTOR);
        encoder_count[1] = rc_encoder_eqep_read(EQEP_LEFT_MOTOR) ;
        printf("EN_R: %lld \t EN_L: %lld\n",encoder_count[0],encoder_count[1]);
        rc_usleep(10000);
    }

//------------------------------------------------------------------------------  
    rc_i2c_close(1);
    rc_encoder_eqep_cleanup();
    rc_remove_pid_file(); // make sure the process is properly ended
    return 0;
}

//namespace motor{
  /**
   * @brief Relevant definitions for the Pololu 37D 24v 141:1 DC motors
   * Most parameters are taken from the official datashet linked below. 
   * According to a pololu engineer (https://forum.pololu.com/t/mechanics-and-electrical-parameters/18153), resistance can be determined as a quotient between the stall current and the rated voltage:
   * I_stall = 2.6 [A],  V_rated = 24 [V] then 
   * R_armor =  V_rated / I_stall = 24 / 2.6 [Ohm] = 9.23 [Ohm] %meassured 17 [ohm]
   * Inductance for this motor series is around 2.4mH
   * L_armor = 2.4mH
   * t_e = L_armor / R_armor = 2.6 ms
   * The electromotive force constant (Ke) can be approximated by dividing the rated voltage by the free-run speed (at the rated voltage).
   * V_rated = 24 [V], w_fr = 79 [rpm],
   * K_e = 24 / 79 [V/RPM] = 0.3038 
   * approximate the motor torque constant (Kt), you can divide the stall torque by the stall current.
   * I_stall = 2.6 [A], \tau_{stall} = 470 [kg mm]  = 4.609 [Nm] (rule of thumb: keept it at 30%: 1.3827[Nm]~=141[kg mm])
   * K_t = 4.609 / 2.6  = 1.77726 [Nm/A] , but for realistic application
   * K_t =  1.3827/0.87  = 1.5893 [Nm/A]
   * @link https://www.pololu.com/file/0J1736/pololu-37d-metal-gearmotors-rev-1-2.pdf @endlink
   * 
   */
 /*
  #define right_wheel_encoder_polarity 1
  #define left_wheel_encoder_polarity 1
  #define right_wheel_gear_box 131
  #define left_wheel_gear_box 131
  #define right_wheel_encoder_resolution 64
  #define left_wheel_encoder_resolution 64
  // stall current: 2.6
  #define datasheet_inductance 2.43e-3 // [H]
  #define datasheet_resistance 9.23 // [ohm]
  #define datasheet_torque_constant 1.5893

  #define right_wheel_encoder_port 1
  #define left_wheel_encoder_port 2

  enum DATASHEET_ELECTRICAL_PARAMETERS: double {
    inductance = 2.43e-3; //[H]
    resistance = 9.23; // [ohm]
    torque_constant = 1.5893; // [Nm/A]
    back_emf_constant = 0.3038 // [V/RPM];
  };*/
  /*enum RIGHT_ENCODER_PARAMETERS: int {
    polarity = 1; 
    gear_box = 131;
    resolution = 64; 
  };
  //weight 120g
  enum LEFT_ENCODER_PARAMETERS: int {
    polarity = 1; 
    gear_box = 131;
    resolution = 64; 
  };
  enum WIRING_DEFINITIONS: int{
    right_encoder_port = 1;
    left_encoder_port = 2;
    right_motor_pwm_port = 1;
    left_motor_pwm_port = 2;
  };
}*/
