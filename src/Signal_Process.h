/*
    High Level Signal processing Sequence
    1. Reset the Maximum velocity value only to trigger at first signal occurence 
    2. Read the Analog value from the sensor
    3. If the Reading Analog value exceeds the 'input_positive_threshold' it regarged as actual signal
    4. Track the value until it starts dropping, at the moment it starts dropping, its max velocity is measured
    5. Set the Maximum velocity
    6. Repeat the 4-5 (multiple hit may occure) until the value drops below (input_negaitve_threshold)
*/

#ifndef Process_Signal_h
#define Process_Signal_h

#include "Arduino.h"


class Signal_Process{
  public:
    //Process_Signal Constructor setups the input pinout, input threshold and velocity steps threshold  
    Signal_Process(int input_pin, int input_threshold, int velocity_threshold);


    int Peak_Detector();  //Return the peak value of the input signal

    //Threshold value setter
    void Set_Input_Threshold(int input_threshold);
    void Set_Velocity_Threshold(int velocity_threshold);



    


  private:
    //Signal Processing Contants
    int analog_input_pin;          //Input pin number
    int schmitt_value = 5;             //Difference between positive threshold and negtiave threshold
    int input_positive_threshold;  //Rising Edge Threshold 
    int input_negaitve_threshold;  //Falling Edge Threshold 
    int velocity_positive_threshold; //Rising Edge Threshold 
    int velocity_negative_threshold;  //Falling Edge Threshold

    int velocity = 0;
    int max_velocity = 0; //Final output variable of the signal processing
    int prev_velocity = 0; 

    //Signal Processing Conditional Variables
    bool sig_process_enabled = false; //This will be true if the sig triggers over the positive threshold 
    bool max_sig_extracted = false; //This will be true only once if Signal_Processing extracted the maximum signal value.


};


#endif