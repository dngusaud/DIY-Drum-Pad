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

float velocity = 0.0;
float max_velocity = 0.0;

int adc_t = 10; //arbitrary ADC sample time in us(?)
int val = 0;
int val_pre = 0;
int slope = 0;

bool peak_detected = false;

Debug_Only scope(115200);
Signal_Process PAD0(A0, 20,20); 

int peakVal = 0;

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
}

void loop() {
  
  // PAD0.Enable_Signal_Processing();

  // if(PAD0.Signal_Present()){
  //   velocity = float(PAD0.Get_Max_Velocity())/1500;
  //   //By setting two playMem for one sample, it allows overlap, 
  //   //otherwise, cracking or pop sound occur due to instant stop of sample
  //   //The pop is still present in long samples eg ride, crash. so its Ideal to have more memory stanby.

  //   if(playMem1.isPlaying()){ 
  //     mixer2.gain(0,velocity);
  //     playMem2.play(Cowbell1);
  //   }
  //   else{
  //     mixer1.gain(0,velocity);
  //     playMem1.play(Cowbell1);
  //   }
  //   scope.test_Print("Velocity : ", velocity);
  // }
  val = analogRead(A0);
  
  if (val > 50){
    if(val > max_velocity + 10){
      max_velocity = val;
      peak_detected = false;
    }
    else if((val < max_velocity - 20) && (peak_detected == false))
    {
      scope.test_Print("Peak: ", val);
      peak_detected = true;
    }
  }
  else{ //Reset
    max_velocity = 0;
    peak_detected = false;
  }


}




