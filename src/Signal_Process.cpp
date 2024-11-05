#include "Arduino.h"
#include "Signal_Process.h"


Signal_Process::Signal_Process(int input_pin, int input_threshold, int velocity_threshold){
    analog_input_pin = input_pin; //Input pin number      
    Set_Input_Threshold(input_threshold);
    Set_Velocity_Threshold(velocity_threshold);
}


void Signal_Process::Set_Input_Threshold(int input_threshold){
    input_positive_threshold = input_threshold;  //Rising Edge Threshold 
    input_negaitve_threshold = input_threshold-schmitt_value;  //Falling Edge Threshold 
}

void Signal_Process::Set_Velocity_Threshold(int velocity_threshold){
    velocity_positive_threshold = velocity_threshold; //Rising Edge Threshold 
    velocity_negative_threshold = velocity_threshold-schmitt_value;  //Falling Edge Threshold  
}



int Signal_Process::Peak_Detector(){

    int raw_value = analogRead(analog_input_pin);

    // if(raw_value < input_negaitve_threshold){   //Signal below negative Threshold, Ignore and return
    //     sig_process_enabled = false;
    //     velocity = 0;
    //     max_velocity = 0;
    //     prev_max_velocity = 0;
    //     peak_detected = false;
    // }

    // if (raw_value > input_positive_threshold){  //Signal detected, proceed next
    //     sig_process_enabled = true;
    //     peak_detected = false;
        
    // }

    // if(raw_value > velocity + velocity_positive_threshold){  //Peak not yet detected, return
    //     velocity = raw_value;
    //     peak_detected = false;
    // }

    // if(raw_value < velocity - velocity_negative_threshold){ //Peak detected, return max velocity
    //     max_velocity = velocity;  
    // } 



}


