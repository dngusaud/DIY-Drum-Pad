#ifndef Debug_Only_h
#define Debug_Only_h

#include "Arduino.h"

class Debug_Only{
    public:
        Debug_Only(int zero);
        //Debugging only, prints value with its name
        void test_Print(String title, float value);
        void test_Print(String title_1, float value_1, String title_2, float value_2);
};

#endif