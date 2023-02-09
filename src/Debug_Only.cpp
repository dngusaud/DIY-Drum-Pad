#include "Arduino.h"
#include "Debug_Only.h"


Debug_Only::Debug_Only(int baudrate){
    Serial.begin(baudrate); //Serial monitoring enabled, recommended to use Arudino Serial Monitor since VSC one is bit laggy
    Serial.println("Debugging Starting");
}

void Debug_Only::test_Print(String title, float value){
    Serial.print(title);
    Serial.print(": ");
    Serial.print(value);
    Serial.println();
}

void Debug_Only::test_Print(String title_1, float value_1, String title_2, float value_2){
    Serial.print(title_1);
    Serial.print(": ");
    Serial.print(value_1);
    Serial.print("  ");
    Serial.print(title_2);
    Serial.print(": ");
    Serial.print(value_2);
    Serial.println();
}