/* DIY DRUM PAD 

  Using the Piezo sensor inputs the knocking signal and outputs processed sound signaㅣ.

  Main Microcontroller: Teensy 4.1 associated with the Audio Extension Board

  Author: Andy Woo

  Teensy Audio Library: https://www.pjrc.com/teensy/gui/index.html
*/



#include <Arduino.h>
#include <Signal_Process.h>
#include <Debug_Only.h>
#include "Samples\AudioSampleCrash.h"
#include "Samples\AudioSampleHihat.h"
#include "Samples\AudioSampleRide.h"
#include "Samples\AudioSampleSnare.h"

#pragma region Teensy Audio Shield
// https://www.pjrc.com/teensy/gui/
//Copied all from the below
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


// GUItool: begin automatically generated code
AudioOutputI2S           i2s1;           //xy=1143,405
AudioPlayMemory          playMem1;       //xy=458,159
AudioPlayMemory          playMem2; //xy=456.99999237060547,195.00000286102295
AudioPlayMemory          playMem3; //xy=458.99999237060547,232.00000381469727
AudioPlayMemory          playMem4; //xy=460.99999237060547,266.9999885559082
AudioPlayMemory          playMem5; //xy=461,319
AudioPlayMemory          playMem6; //xy=459.99999237060547,355.00000286102295
AudioPlayMemory          playMem7;   //xy=461.99999237060547,392.00000381469727
AudioPlayMemory          playMem8; //xy=463.99999237060547,426.9999885559082
AudioPlayMemory          playMem9; //xy=463,481
AudioPlayMemory          playMem10; //xy=461.99999237060547,517.000002861023
AudioPlayMemory          playMem11; //xy=463.99999237060547,554.0000038146973
AudioPlayMemory          playMem12; //xy=465.99999237060547,588.9999885559082
AudioMixer4              mixer1;         //xy=734.0000267028809,212.00000381469727
AudioMixer4              mixer2; //xy=733,366
AudioMixer4              mixer3; //xy=732,529
AudioMixer4              mixer4; //xy=934,406
AudioConnection          patchCord1(playMem2, 0, mixer1, 1);
AudioConnection          patchCord2(playMem1, 0, mixer1, 0);
AudioConnection          patchCord3(playMem3, 0, mixer1, 2);
AudioConnection          patchCord4(playMem6, 0, mixer2, 1);
AudioConnection          patchCord5(playMem4, 0, mixer1, 3);
AudioConnection          patchCord6(playMem5, 0, mixer2, 0);
AudioConnection          patchCord7(playMem7, 0, mixer2, 2);
AudioConnection          patchCord8(playMem10, 0, mixer3, 1);
AudioConnection          patchCord9(playMem9, 0, mixer3, 0);
AudioConnection          patchCord10(playMem8, 0, mixer2, 3);
AudioConnection          patchCord11(playMem11, 0, mixer3, 2);
AudioConnection          patchCord12(playMem12, 0, mixer3, 3);
AudioConnection          patchCord13(mixer3, 0, mixer4, 2);
AudioConnection          patchCord14(mixer2, 0, mixer4, 1);
AudioConnection          patchCord15(mixer1, 0, mixer4, 0);
AudioConnection          patchCord16(mixer4, 0, i2s1, 0);
AudioConnection          patchCord17(mixer4, 0, i2s1, 1);
// GUItool: end automatically generated code

// Create an object to control the audio shield.
AudioControlSGTL5000 audioShield;

#pragma endregion

Debug_Only scope(9600);
Signal_Process PAD0(A0,10, 20); 
Signal_Process PAD1(A1,10, 20); 

int analog_input_pin;          //Input pin number
int input_hysteresis = 5;             //Difference between positive threshold and negtiave threshold
int velocity_hysteresis = 5;             //Difference between positive threshold and negtiave threshold
int input_pos_thres;  //Rising Edge Threshold 
int input_neg_thres;  //Falling Edge Threshold 
int vel_pos_thres; //Rising Edge Threshold 
int vel_neg_thres;  //Falling Edge Threshold
int raw;


int max_velocity = 0; //Final output variable of the signal processing

//Signal Processing Conditional Variables
bool peak_detected = 0; // 0: stand by expecting peak, 1: peak detected

void setup() {
  AudioMemory(10);
  
  // turn on the output
  audioShield.enable();
  audioShield.volume(0.7);

  // reduce the gain on mixer channels, so more than 1
  // sound can play simultaneously without clipping
  mixer1.gain(0, 0.7);

  Serial.print("Serial Begin");
  pinMode(0,INPUT_PULLUP);
}

void loop() {
  int val = PAD0.Peak_Detector();
  int val2 = PAD1.Peak_Detector();

  if(val > 0){
    //By setting two playMem for one sample, it allows overlap, 
    //otherwise, cracking or pop sound occur due to instant stop of sample
    //The pop is still present in long samples eg ride, crash. so its Ideal to have more memory stanby.
    float volume = ((float)val / 824);
    mixer1.gain(0,volume * 1.2);
    playMem1.play(AudioSampleRide);
    Serial.println("Peak1 : " + String(volume));
  }

  if(val2 > 0){
    float volume2 = ((float)val2 / 824);
    mixer1.gain(1,volume2 * 0.4);
    playMem2.play(AudioSampleSnare);
    Serial.println("Peak2 : " + String(volume2));
  }
}
 // Cowbell audio sampling tested. It sucessfully plays the cow bell sample sound with impact the piezo dynamically. No delay or skipping input detected. 
 // Small issue as if multiple input received, sometime, often called "pop" sound detected. We need better audio playing algorithm to implement auto envlope at start and end of sample play.
 