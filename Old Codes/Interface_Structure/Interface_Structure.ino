
#define Button_1_Pin 8
#define Button_2_Pin 9
#define Button_3_Pin 10

//General
char buffer[10];

// General Slidng Window vars
// Since there is only 4 lines available in the LCD screen, to contain more contents in it, the whole window should slide up and down like a website scrolling
int index = 10; //Number of contents that will be contained in the LCD
const int window_width = 4; //Maximum number of lines in LCD (16x2 and 20x4 possible)
char *list[20][20] = {{},{}};  //The final buffer array before it prints from the LCD
char *window[window_width][window_width] = {{"","","",""},{"","","",""}};   //Actual window that prints on the screen
char cursor_pos[window_width] = {'X','O','O','O'};   //Cursor that will be displayed on LCD
int current_row_pos = 0;  //Current row position from top to index number row
int window_left_pos = 0;  //Indicate where the window left edge is 
int window_right_pos = 3; //Indicate where the window right edge is  
int last_window_left_pos = 0; //Previous frame left window edge
int last_window_right_pos = 3; // Previous frame right window edge
bool exceed_edge = 0; //True if the cursor wants to move across border of the window



// Menu Browsing
int current_main_menu = 0; // Current_Menu Position
int current_sub_menu = -1; //Current_sub_Menu Position
int current_config_menu = -1;

bool at_main_menu = 1;
bool at_sub_menu = 0; 
bool at_config = 0;


// Main Menu
const int main_menu_num = 3;
char *main_menu[main_menu_num] = {"Sample Mapping" ,"Volume Setting" ,"Threshold Setting"}; // Array of elements in main menu

//First Sub Menu : Sample Mapping
const int sample_num = 11;  //Number of samples that can be mapped to each input pad/bar/pedal
char *input_pads[sample_num] = {"PAD1: ","PAD2: ","PAD3: ","PAD4: ","PAD5: ","PAD6: ","BAR1: ","BAR2: ","BAR3: ","EXT1:","EXT2: "};
char *current_samples[sample_num] = {"Kick1","Snare1","Hihatclosed1","Ride1","Crash1","Ride2","Snare2","Kick2","Snare3","Kick1","Ride1"};

//Sample Mapping Config
const int sample_list_num = 15; //Number of preset sample for drum
char *sample_list_name[sample_list_num]   = {"Kick1", "Kick2", "Kick3", "Snare1", "Snare2", "Snare3", "Hihatclosed1", "Hihatclosed2", "Hihatclosed3", "Ride1", "Ride2", "Ride3", "Crash1", "Crash2", "Crash3"};
char *none[19] = {};


//Second Sub Menu : Volume Setting
const int volume_num = 12;  //Number of volume setting each 11 input pads and 1 master volume
char *volume_list[volume_num] = {"Master: ","PAD1: ","PAD2: ","PAD3: ","PAD4: ","PAD5: ","PAD6: ","BAR1: ","BAR2: ","BAR3: ","EXT1:","EXT2: "};
char *volume_val[volume_num] = {"80"  ,"80"  ,"80"  ,"80"  ,"80"  ,"80"  ,"80"  ,"80"  ,"80"  ,"80"  ,"80"  ,"80"  };  //Each volume values

//Volume Setting Config
const int volume_list_num = 2;




//Third Sub Menu : Threshold Setting 
const int threshold_num = 11;  //Number of volume setting each 11 input pads and 1 master volume
char *threshold_list[threshold_num] = {"PAD1: ","PAD2: ","PAD3: ","PAD4: ","PAD5: ","PAD6: ","BAR1: ","BAR2: ","BAR3: ","EXT1:","EXT2: "};
char *threshold_val[threshold_num] = {"30"  ,"30"  ,"30"  ,"30"  ,"30"  ,"30"  ,"30"  ,"30"  ,"30"  ,"30"  ,"30"  };  //Each volume values

//Threshold Setting Config





// Button Input
int button_val = 0; //-1: Left, 1: Right, 0: None, 2: Middle Press
int dir = 0; //-1: Left, 1: Right, 0: None
int mid_press = 0; 
int last_button_val = -1;




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Button_1_Pin,INPUT_PULLUP);
  pinMode(Button_2_Pin,INPUT_PULLUP);
  pinMode(Button_3_Pin,INPUT_PULLUP);

  
}

void loop() {
  //Input Process
  Read_Button_Val();  // Reads button input
  Falling_Edge();     // Specify the falling edge of input signal



  // Main Algorithm
  Set_List();      //Set the contents in list depend on current menu position
  Current_Pos();      // Calculate the current position from the list
  Slide_Cursor();     // Change cursor position
  Set_Window();     // set Window 



  //Debug Only
  Print_Values();
  New_Line();
  Print_Cursor(4,cursor_pos);
  Serial.print("  ");
  Serial.print(current_row_pos);
  New_Line();
}





//Takes two char array to insert them into the final buffer
//If the window is at the very top, it should print 'return' to go back to previous menu
void Insert_2_List(char **col1, char **col2, int index, bool isReturn){  
  if(isReturn){
    list[0][0] = "Return";
    list[1][0] = "";
  }
  for(int i = 0; i<index; i++){
    list[0][i+isReturn] = col1[i];
  }
  for(int i = 0; i<index; i++){
    list[1][i+isReturn] = col2[i];
  }
}


void Main_Menu(){
  Insert_2_List(main_menu,none,main_menu_num,0);
}

void Sample_Mapping(){  //Show the current sample mapping to each input sensors
  Insert_2_List(input_pads,current_samples,sample_num,1);
}

void Volume_setting(){  //Show the current volume setting
  Insert_2_List(volume_list,volume_val,volume_num,1);
}

void Threshold_setting(){   //Show the current threshold setting
  Insert_2_List(threshold_list,threshold_val,threshold_num,1);
}



void Sample_Mapping_Config(){ //Change the sample mapping
  Insert_2_List(sample_list_name,none,sample_list_num,0);
  if(mid_press){    //User selects the sample for the pads
    current_samples[current_sub_menu-1] = sample_list_name[current_row_pos];
    at_main_menu = 0;
    at_sub_menu = 1;
    at_config = 0;
    index = 20;
    window_left_pos = last_window_left_pos;
    window_right_pos = last_window_right_pos;
    Set_Cursor_Pos(current_sub_menu-window_left_pos);
    Clear_List();
  }
}

void Volume_Setting_Config(){
  list[0][0] = volume_list[current_sub_menu-1];
  list[0][1] = volume_val[current_sub_menu-1];
  int num = atoi(volume_val[current_sub_menu-1]);
  if(dir != 0){
    num =  constrain(dir + num, 0, 100);
    sprintf(buffer,"%d",num);
    volume_val[current_sub_menu-1] = buffer;
  }
  
  if(mid_press){    //User selects the sample for the pads
    at_main_menu = 0;
    at_sub_menu = 1;
    at_config = 0;
    index = 20;
    window_left_pos = last_window_left_pos;
    window_right_pos = last_window_right_pos;
    Set_Cursor_Pos(current_sub_menu-window_left_pos);
    Clear_List();
  }
}


void Threshold_Setting_Config(){
  list[0][0] = threshold_list[current_sub_menu-1];
  list[0][1] = threshold_val[current_sub_menu-1];
  int num = atoi(threshold_val[current_sub_menu-1]);
  if(dir != 0){
    num =  constrain(dir + num, 0, 100);
    sprintf(buffer,"%d",num);
    threshold_val[current_sub_menu-1] = buffer;
  }
  
  if(mid_press){    //User selects the sample for the pads
    at_main_menu = 0;
    at_sub_menu = 1;
    at_config = 0;
    index = 20;
    window_left_pos = last_window_left_pos;
    window_right_pos = last_window_right_pos;
    Set_Cursor_Pos(current_sub_menu-window_left_pos);
    Clear_List();
  }
}




void Set_List(){    //Actual browsing happen here,
  if(at_main_menu){
    index = main_menu_num;  
    current_main_menu = current_row_pos;
    current_sub_menu = -1;
    Main_Menu();
    if(mid_press){  //Entering a Sub menu
      at_main_menu = 0;
      at_sub_menu = 1;
      at_config = 0;
      Reset_Window();
      Clear_List();
    }
  }
  else if(at_sub_menu){  
    current_sub_menu = current_row_pos;
    current_config_menu = -1;
    if(current_main_menu == 0){ //Sample Mapping
      index = sample_num + 1;
      Sample_Mapping();
    }
    else if(current_main_menu == 1){  //Volume Setting
      index = volume_num + 1;
      Volume_setting();
    }
    else if(current_main_menu == 2){  //Threshold Setting
      index = threshold_num + 1;
      Threshold_setting();
    }
    
    if(mid_press && current_sub_menu != 0){   //Entering the Config Menu
      at_main_menu = 0;
      at_sub_menu = 0;
      at_config = 1;
      last_window_left_pos = window_left_pos;
      last_window_right_pos = window_right_pos;
      Reset_Window();
      Clear_List();
    }
    else if(mid_press && current_sub_menu == 0){  //Going back to Main Menu
      at_main_menu = 1;
      at_sub_menu = 0;
      at_config = 0;
      index = 20;
      Set_Cursor_Pos(current_main_menu);
      Clear_List();
    }
  }
  else if(at_config){
    index = 2;
    current_config_menu = current_row_pos;
    
    if(current_main_menu == 0){ //Sample Mapping
      index = sample_list_num;
      Sample_Mapping_Config();
    }
    else if(current_main_menu == 1){  //Volume Setting
      index = 1;
      Volume_Setting_Config();
    }
    else if(current_main_menu == 2){  //Threshold Setting
      index = 1;
      Threshold_Setting_Config();
    }
  }
}


void Current_Pos(){
  current_row_pos = window_left_pos + Index_from_Char('X',4,cursor_pos);
}

void Clear_List(){
  for(int i = 0; i<99; i++){
    list[0][i] = "";
    list[1][i] = "";
  }
}


void Set_Window(){  //set the window in the list, when the cursor wants to exceed the edge, it slides the window
  if(exceed_edge){
    if((window_left_pos != 0 || dir != -1) && (window_right_pos != index-1 || dir != 1)){ //Only slide in valid condition (eg do not exceed list)
      window_left_pos += dir;
      window_right_pos += dir;
    }
  }
  for(int i = 0; i<4; i++){
    window[0][i] = list[0][window_left_pos + i];
    window[1][i] = list[1][window_left_pos + i];
  }
}



void Reset_Window(){
  window_left_pos = 0;
  window_right_pos = 3;
  Set_Cursor_Pos(0);
}

void Set_Cursor_Pos(int pos){
  for(int i=0; i<4; i++){
    if(i==pos){
      cursor_pos[i] = 'X';
    }
    else{
      cursor_pos[i] = 'O';
    }
  }
}


void Slide_Cursor(){  //Slides the cursor in between 0~3 index, when it wants to exceed the edge, the exceed_edge goes high.
  int current_cursor_pos = Index_from_Char('X',4,cursor_pos);
  int new_cursor_pos = current_cursor_pos + dir;
  if(index >4){
    if(new_cursor_pos <0){
      exceed_edge = 1;
      new_cursor_pos = 0;
    }
    else if(new_cursor_pos > 3){
      exceed_edge = 1;
      new_cursor_pos = 3;
    }
    else{
      exceed_edge = 0;
    }
  }
  new_cursor_pos = constrain(new_cursor_pos,0,index-1);
  cursor_pos[current_cursor_pos] = 'O';
  cursor_pos[new_cursor_pos] = 'X';
}


// Takes inputs: target char, length of array and array pointer, Outputs the index of the first occurence of the target.
int Index_from_Char(char e, int len , char *list){
  for(int i = 0; i<len; i++){
    if(e == list[i]){
      return i;
    }
  }
}



void Read_Button_Val(){ //Mock signal of Encoder, left button = left turn of the enocder and vice versa
  if(!digitalRead(Button_1_Pin)){
    button_val = -1;
  }
  else if(!digitalRead(Button_2_Pin)){
    button_val = 2;
  }
  else if(!digitalRead(Button_3_Pin)){
    button_val = 1;
  }
  else{
    button_val = 0;
  }
}


void Falling_Edge(){  //Only trigger value when falling edge (since it is input pullup)
  if(last_button_val != button_val){
    if(button_val != 0){
      if(button_val == 2){
        mid_press = 1;
      }
      else{
        dir = button_val;
      }
    }
  }
  else{
    dir = 0;
    mid_press = 0;
  }
  last_button_val = button_val;
}

void Print_Values(){
  for(int r = 0; r<4; r++){
    for(int c = 0; c<2; c++){
      Serial.print(window[c][r]);
      Serial.print(" ");
    }
    New_Line();
  }
}

void Print_Cursor(int len, char *list){
  for(int i = 0; i<len; i++){
    Serial.print(list[i]);
    Serial.print(",");
  }
}

void New_Line(){
  Serial.println();
}
