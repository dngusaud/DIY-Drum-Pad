//  When exporting from Audacity, please follow these:
//  Save as type  : WAV(Microsoft)
//  Encoding      : Singed 16-bit PCM
//  Otherwise if you convert to sketch by Wav2Sketch.eve it will print nothing.

#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Audio.h>
#include <Wire.h>
#include <Bounce.h>


// Define
#define Encoder_1_L_PIN 30
#define Encoder_1_R_PIN 31
#define Encoder_1_B_PIN 32
#define Encoder_2_L_PIN 27
#define Encoder_2_R_PIN 28
#define Encoder_1_B_PIN 29


// Samples path
const int sample_list_num = 15; //Total number of samples
#include "src/Sample_List/Kick1.h"
#include "src/Sample_List/Kick2.h"
#include "src/Sample_List/Kick3.h"
#include "src/Sample_List/Snare1.h"
#include "src/Sample_List/Snare2.h"
#include "src/Sample_List/Snare3.h"
#include "src/Sample_List/Hihatclosed1.h"
#include "src/Sample_List/Hihatclosed2.h"
#include "src/Sample_List/Hihatclosed3.h"
#include "src/Sample_List/Ride1.h"
#include "src/Sample_List/Ride2.h"
#include "src/Sample_List/Ride3.h"
#include "src/Sample_List/Crash1.h"
#include "src/Sample_List/Crash2.h"
#include "src/Sample_List/Crash3.h"

// GUItool: begin automatically generated code
AudioPlayMemory          playMem2;       //xy=315.00000381469727,572.0000095367432
AudioPlayMemory          playMem3;       //xy=315.00000762939453,608.0000104904175
AudioPlayMemory          playMem1;       //xy=316.00000381469727,537.0000085830688
AudioPlayMemory          playMem4;       //xy=317.00000762939453,643.0000104904175
AudioMixer4              mixer1;         //xy=538.0000076293945,583.0000076293945
AudioMixer4              mixer2; //xy=671,562
AudioOutputI2S           i2s1;           //xy=855.0000114440918,531.0000076293945
AudioConnection          patchCord1(playMem2, 0, mixer1, 1);
AudioConnection          patchCord2(playMem3, 0, mixer1, 2);
AudioConnection          patchCord3(playMem1, 0, mixer1, 0);
AudioConnection          patchCord4(playMem4, 0, mixer1, 3);
AudioConnection          patchCord5(mixer1, 0, mixer2, 3);
AudioConnection          patchCord6(mixer2, 0, i2s1, 0);
AudioConnection          patchCord7(mixer2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=513.0000076293945,400.0000047683716
// GUItool: end automatically generated code


// Button setup
Bounce button0 = Bounce(32, 5);  // 50 ms debounce time
Bounce button1 = Bounce(33, 5);
Bounce button2 = Bounce(34, 5);  // 50 ms debounce time
Bounce button3 = Bounce(35, 5);
Bounce button4 = Bounce(36, 5);


//LCD Setup
LiquidCrystal_I2C lcd(0x26, 20, 4); // I2C address 0x27, 16 column and 2 rows


//Encoder Setup
Encoder enc_1(Encoder_1_L_PIN, Encoder_1_R_PIN);

long oldPosition_1 = -999;
long newPosition_1 = 0;
const int section_num = 11;

int current_row = 0; //Where the editor is pointing in the list
int current_cursor_row = 0; //where the real cursor pointing in LCD
int last_cursor_row = -1;   //where the last real cursor pointed in the LCD
int current_window_top = 0; //where is the top edge of the current LCD
int lcd_mode = 0; //0: Main Menu, 1:Sample Mapping  2:Sample Selection  3:Volume Setting  4:Threshold Setting  
char* lcd_window[2][4] = {{'1','3','5','7'},{'2','4','6','8'}};

//Main Menu
const int menu_num = 3;
char *menus[menu_num] = {"Sample Mapping", "Volume Setting", "Threshold Setting"};




//Sample Mapping 
char *section[section_num] = {"PAD1: ","PAD2: ","PAD3: ","PAD4: ","PAD5: ","PAD6: ","BAR1: ","BAR2: ","BAR3: ","EXT1:","EXT2: "};
char *sample[section_num]   = {"Kick1","Snare1","Hihatclosed1","Ride1","Crash1","Ride2","Snare2","Kick2","Snare3","Kick1","Ride1"};
//const unsigned int sample_list[sample_list_num]   = {Kick1, Kick2, Kick3, Snare1, Snare2, Snare3, Hihatclosed1, Hihatclosed2, Hihatclosed3, Ride1, Ride2, Ride3, Crash1, Crash2, Crash3};
char *sample_list_name[sample_list_num]   = {"Kick1", "Kick2", "Kick3", "Snare1", "Snare2", "Snare3", "Hihatclosed1", "Hihatclosed2", "Hihatclosed3", "Ride1", "Ride2", "Ride3", "Crash1", "Crash2", "Crash3"};

//TESTing




// SD card pin setup
#define SDCARD_CS_PIN   10
#define SDCARD_MOSI_PIN 7
#define SDCARD_SCK_PIN  14

// Volume knob setup
#define MASTER_VOL_PIN  A17 


//Variables
float master_vol;

int k = 0;
int last_k = -1;

void setup() {
  Audio_Setup();
  Button_Setup(); 
  LCD_Setup();
  Serial.begin(9600);
}

void loop() {
  Button_Update();
  encoder_1_pos();



  last_k = k;
}

//********************************* Setup Functions **********************************************//

//LCD Setup function, initialize LCD and set cursor at home
void LCD_Setup(){
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.setCursor(6, 1);  
  lcd.print("WELCOME");
  lcd.setCursor(6, 2);    
  lcd.print("MY DRUM");
  delay(2000);
  lcd.clear();
  lcd.home();
}


//Main Audio board setup
void Audio_Setup(){
  AudioMemory(12);
  
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.7);

  mixer1.gain(0, 0.5);
  mixer1.gain(1, 0.2);
  mixer1.gain(2, 0.2);
  mixer1.gain(3, 0.5);
  mixer2.gain(1, 0.5);
  mixer2.gain(0, 0.5);
  mixer2.gain(3, 0.5); 
  
  delay(100);
}


//Button pinout setup
void Button_Setup(){
  pinMode(30, INPUT_PULLUP);
  pinMode(31, INPUT_PULLUP);
  pinMode(32, INPUT_PULLUP);
  pinMode(33, INPUT_PULLUP);
  pinMode(34, INPUT_PULLUP);
  pinMode(35, INPUT_PULLUP);
  pinMode(36, INPUT_PULLUP);
}



//************************************ Functions *************************************************//
void Button_Update(){
  button0.update();
  button1.update();
  button2.update();
  button3.update();
  button4.update();
}


void Initialize_LCD_POS(){
    current_row = 0; 
    current_cursor_row = 0; 
    last_cursor_row = -1;  
    current_window_top = 0; 
}

void Initialize_LCD(){
  lcd.home();
  lcd.clear();
}


void Move_cursor(){
  current_row = constrain(current_row + k,0,section_num);
  current_cursor_row = constrain(current_cursor_row + k,0,3);
  if(current_cursor_row == last_cursor_row){
    if(current_cursor_row == 3 || current_cursor_row == 0){
      current_window_top = constrain(current_window_top + k, 0, section_num-4);
    }
  }
}


void Main_Menu(){
  lcd_mode = 0;
  Initialize_LCD();

  for(int c = 0; c<2; c++){
    for(int r = 0; r<menu_num; r++){
      if(r == current_cursor_row){  //Set the cursor position
        lcd.setCursor(c,r);
        lcd.print("-"); 
        }
    }
  }
  
}
//-------------- Last Update, The sample selection part is partially working, now have to use the encoder button to enter and leave sub and main menus 

void display_lcd(){  // Shows the list of the binding of pads to the samples
    Initialize_LCD();
    lcd_mode = 1;
    for(int r = 0; r<4; r++){
      for(int c = 0; c<3; c++){
        if(c ==0){
          if(r == current_cursor_row){  //Set the cursor position
            lcd.setCursor(c,r);
            lcd.print("-"); 
          }
        }
        else if(c==1){
          lcd.setCursor(c,r);
          lcd.print(lcd_window[0][r]);
        }
        else if(c==2){
          lcd.setCursor(c+5,r);
          lcd.print(lcd_window[1][r]);
      }
    }
  }
}


void Sample_Mapping_Move_Window(){ // Slide up and down the list once it hits the edge. 
  if(last_cursor_row == current_cursor_row){
    if(current_cursor_row == 3 || current_cursor_row == 0){
        for(int r = 0; r<4; r++){
          lcd_window[0][r] = section[current_window_top + r];
          lcd_window[1][r] = sample[current_window_top + r];
        }
    }
  }
  last_cursor_row = current_cursor_row;
}



void Sample_Mapping_Select_Sample(){
  if(button0.fallingEdge() && lcd_mode == 1){
    Initialize_LCD_POS();
    Initialize_LCD();
    lcd_mode == 2;
    lcd.print("List of Samples");
  }
}





//Set the master ouput volume throguh potentiometer
void Master_Volume_Knob(){
  
  master_vol = map(analogRead(MASTER_VOL_PIN),0,1023,0,100); //Step down range to 0~100;
  master_vol = master_vol/100; //Step one more down to 0~1;
  
  sgtl5000_1.volume(master_vol); //Set the master volume
  //Serial.println(vol);  Debug Only
}


void encoder_1_pos(){
  newPosition_1 = enc_1.read()/4;
  if (newPosition_1 != oldPosition_1) {
    if (newPosition_1 < oldPosition_1){
      k = 1;
    }
    else if (newPosition_1 > oldPosition_1){
      k = -1;
    }
    oldPosition_1 = newPosition_1;
  }
  else{
    k = 0;
  }
}
