/* DIY DRUM PAD 

  Using the Piezo sensor inputs the knocking signal and outputs processed sound signaㅣ.

  Main Microcontroller: Teensy 4.1 associated with the Audio Extension Board

  Author: Andy Woo

  Teensy Audio Library: https://www.pjrc.com/teensy/gui/index.html
*/



#include <Arduino.h>
#include <Signal_Process.h>
#include <Debug_Only.h>
#include "Cowbell1.h"  //Test Sample

#pragma region Teensy Audio Shield
// https://www.pjrc.com/teensy/gui/
//Copied all from the below
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioPlayMemory          playMem1;       //xy=215,209.00000381469727
AudioPlayMemory          playMem2;       //xy=216.00000381469727,290.00000381469727
AudioMixer4              mixer2;         //xy=475.00000762939453,300.00000381469727
AudioMixer4              mixer1;         //xy=480.00000762939453,235.00000381469727
AudioMixer4              mixer3;         //xy=636.0000095367432,262.00000381469727
AudioOutputI2S           i2s1;           //xy=811.0000114440918,236.00000381469727
AudioConnection          patchCord1(playMem1, 0, mixer1, 0);
AudioConnection          patchCord2(playMem2, 0, mixer2, 0);
AudioConnection          patchCord3(mixer2, 0, mixer3, 1);
AudioConnection          patchCord4(mixer1, 0, mixer3, 0);
AudioConnection          patchCord5(mixer3, 0, i2s1, 0);
AudioConnection          patchCord6(mixer3, 0, i2s1, 1);

// Copied all from above

// Create an object to control the audio shield.
AudioControlSGTL5000 audioShield;

#pragma endregion

Debug_Only scope(9600);
Signal_Process PAD0(A0,15, 10); 


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
  audioShield.volume(0.5);

  // reduce the gain on mixer channels, so more than 1
  // sound can play simultaneously without clipping
  mixer1.gain(0, 0.7);
  mixer2.gain(0, 0.7);

  Serial.print("Serial Begin");
  pinMode(0,INPUT_PULLUP);
}

void loop() {
  int val = PAD0.Peak_Detector();

  if(val > 0){
    //By setting two playMem for one sample, it allows overlap, 
    //otherwise, cracking or pop sound occur due to instant stop of sample
    //The pop is still present in long samples eg ride, crash. so its Ideal to have more memory stanby.
    float volume = ((float)val / 624);
    mixer2.gain(0,volume);
    playMem2.play(Cowbell1);

    Serial.println("Peak: " + String(volume));
  }
}
 // Cowbell audio sampling tested. It sucessfully plays the cow bell sample sound with impact the piezo dynamically. No delay or skipping input detected. 
 // Small issue as if multiple input received, sometime, often called "pop" sound detected. We need better audio playing algorithm to implement auto envlope at start and end of sample play.
 