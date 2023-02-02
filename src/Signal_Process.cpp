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

//This function should constantly run in the Main Loop() to monitor the input signal
void Signal_Process::Enable_Signal_Processing(){
    Reset_Max_Velocity();           //Reset the Max Velocity value
    val = Read_Input_Signal(analog_input_pin);  //Constanly reads the input signal
    Monitor_Input_Signal(val);      //If the signal is greater than the threshold value, enbale the signal processing
    Extract_Max_Velocity(val);      //Process the input signal, extract the maximum value
}

bool Signal_Process::Signal_Present(){
    return (max_velocity != 0);
}

//Outputs the maximum velocity measured
int Signal_Process::Get_Max_Velocity(){
    return max_velocity;
}


//------Below is the Private Functions----------


//Setup the pinout for the signal processing
int Signal_Process::Read_Input_Signal(int pin_num){
    return analogRead(pin_num);
}


//Monitor if the reading signal is greater than the threshold value, virtual schmitt trigger is applied
void Signal_Process::Monitor_Input_Signal(int value){
    if(!sig_process_enabled){
    if (value > input_positive_threshold){  //Signal processing enabled
        sig_process_enabled = true;
    }
    }

    if(sig_process_enabled){
    if(value < input_negaitve_threshold){   //signal processing disabled
        sig_process_enabled = false;
    }
    }
}


// Extract the maximum velocity
void Signal_Process::Extract_Max_Velocity(int value){
    if(sig_process_enabled){
    if(value > velocity + velocity_positive_threshold){  //If the input value is greater than the recorded max value + extra threshold, set signal as max value
        velocity = value;
        max_sig_extracted = false;
    }
    if(value < velocity - velocity_negative_threshold){
        if(!max_sig_extracted){ //Setting Max velocity triggered
        max_sig_extracted = true;
        max_velocity = velocity + velocity_negative_threshold; //The maximum velocity detected when raw value drops 5 again velocity. 
        }
        else{//When maximum velocity is already set keep track the value
        velocity = value;
        }
    } 
    prev_velocity = velocity;
    }
}

//Reset the maximum velocity to 0
void Signal_Process::Reset_Max_Velocity(){  
    max_velocity = 0;
}
