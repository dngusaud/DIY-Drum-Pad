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


    raw = analogRead(analog_input_pin);

    if(raw > 20){
        if(!processing){
            processing = true;
            Serial.println("---------------------START--------------------");
        }
        Serial.println("RAW: " + String(raw) + "    State: " + String(peak_detected) + "    Peak: " + String(max_velocity) );
        if((raw > max_velocity + 20) && (peak_detected == 0)){
            max_velocity = raw;
            peak_detected = 0;
        }
        
        if((raw < max_velocity - 35) && (peak_detected == 0)){
            peak_detected = 1;
            Serial.print("--------------------------------------------------------------");
        }

        if((raw > prev_raw + 10) && (peak_detected == 1)){
            max_velocity = 0;
            peak_detected = 0;
        }
    }
    else if(raw < 5){
        max_velocity = 0;
        peak_detected = 0;
        raw = 0;
        processing = false;
    }
    prev_raw = raw;
    return 0;
}


// Current Bug, 1. It is not truely dynamically detecing input
// Need to check if that dynamic is represented in the raw (priobably yes)
// Peak detection reset error afer max = 0, immediately update peak, maybe okay since detection is not flaggeD?


