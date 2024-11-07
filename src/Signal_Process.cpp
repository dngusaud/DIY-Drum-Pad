#include "Arduino.h"
#include "Signal_Process.h"


Signal_Process::Signal_Process(int input_pin, int input_threshold, int velocity_threshold){
    analog_input_pin = input_pin; //Input pin number      
    Set_Input_Threshold(input_threshold, input_hysteresis);
    Set_Velocity_Threshold(velocity_threshold, velocity_hysteresis);
}


void Signal_Process::Set_Input_Threshold(int input_threshold, int hysteresis){
    input_pos_thres = input_threshold;  //Rising Edge Threshold 
    input_neg_thres = input_threshold - hysteresis;  //Falling Edge Threshold 
}

void Signal_Process::Set_Velocity_Threshold(int velocity_threshold, int hysteresis){
    vel_pos_thres = velocity_threshold; //Rising Edge Threshold 
    vel_neg_thres = velocity_threshold + hysteresis;  //Falling Edge Threshold  
}

int Signal_Process::Peak_Detector(){

    int thres = 15;

    raw = analogRead(analog_input_pin);

    if(raw > 20){
        if ((raw > filtered_val + thres) or (raw  < filtered_val - thres)){   //filter the signal with step of 10;
            filtered_val = raw;
            if((prev_slope > 0) && ((filtered_val - prev_filtered_val) < 0)){
                max_velocity = filtered_val + thres;
                Serial.println("Peak: " + String(max_velocity));
            }
            //Serial.println("Current Slope: " + String(filtered_val - prev_filtered_val) + "    prev Slope: " + String(prev_slope));
            prev_slope = filtered_val - prev_filtered_val;
            prev_filtered_val = filtered_val;
        }
    }
    else if(raw < 10){
        max_velocity = 0;
        detection_state = 0;
        filtered_val = 0;
        prev_filtered_val = 0;
        prev_slope = 0;
        processing = false;  
    }
    return 0;
}

// Dynamic detection is achieved, now need a method to return this value, to only return value the peak values once. Not sequence of themm.....

