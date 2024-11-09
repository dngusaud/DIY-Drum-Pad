#include "Arduino.h"
#include "Signal_Process.h"


Signal_Process::Signal_Process(uint8_t input_pin, uint8_t input_deadband, uint8_t velocity_deadband){
    analog_input_pin = input_pin; //Input pin number      
    Set_Input_Deadband(input_deadband);
    Set_Velocity_Deadband(velocity_deadband);
}

void Signal_Process::Set_Input_Deadband(uint8_t deadband){
    input_deadband_thres = deadband;    //Value greater than this value from ADC output will be counted as valid signal
}

void Signal_Process::Set_Velocity_Deadband(uint8_t velocity_deadband){
    velocity_thres = velocity_deadband;   //Will be used for the peak detection method
}

int Signal_Process::Peak_Detector(){

    raw = analogRead(analog_input_pin);

    if(raw > input_deadband_thres){   //Signal detected extract peak value
        if ((raw > filtered_val + velocity_thres) or (raw  < filtered_val - velocity_thres)){   //Deadband filter threshold detection
            filtered_val = raw;
            if((prev_slope > 0) && ((filtered_val - prev_filtered_val) < 0)){   //Slope change from positive to negative, meaning local peak has been reached
                max_velocity = filtered_val + velocity_thres;
            }
            else{
                max_velocity = 0;
            }
            prev_slope = filtered_val - prev_filtered_val;  // Save current slope to compare at next
            prev_filtered_val = filtered_val;   // Save current value for filtering 
        }
        else{
            max_velocity = 0;
        }
    }
    else if(raw < input_deadband_thres - input_hystersis){  // Signal below threshold, ignore as noise
        max_velocity = 0;
        filtered_val = 0;
        prev_filtered_val = 0;
        prev_slope = 0;
    }

    return max_velocity;
}