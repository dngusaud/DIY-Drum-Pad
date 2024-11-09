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
    Signal_Process(uint8_t input_pin, uint8_t input_deadband, uint8_t velocity_levels);

    int Peak_Detector();  //Return the peak value of the input signal

    //Filtering Setter
    void Set_Input_Deadband(uint8_t input_deadband);
    void Set_Input_Maximum(uint8_t input_maximum);
    void Set_Velocity_Deadband(uint8_t velocity_deadband);
    

  private:    
    // raw ADC signal variables
    int analog_input_pin; // ADC Input pin number
    int raw = 0;          // Raw ADC value
    int input_deadband_thres; // Enage input threshold 
    int input_hystersis = 5;  // Release hystersis

    // Peak Detection variables
    int velocity_thres;   // Deadband value for filter
    int filtered_val = 0; // Filtered value using deadband filter 
    int prev_filtered_val = 0;  // previous filtered value to extract current slope
    int prev_slope = 0;   // Previous slope value.  
    int max_velocity = 0; //Final output variable of the signal processing
};

#endif