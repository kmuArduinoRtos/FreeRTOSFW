/*
    RTOS Framework for Arduino
    Copyright (c) 2016, School of Computer Enge. Kookmin Univ., Seoul, Korea
    All rights reserved
*/

#include <framework.h>

void print_A() {
    Serial.println("A");
}

void print_B() {
    Serial.println("B");
}

void print_C() {
    Serial.println("C");
}

void print_D() {
    Serial.println("D");
}

//------------------------------------------------------------------------------
void setup() {
    Serial.begin(9600);

// MINSUK --> 아래 주기를 더 차이나게 하면 좋지 않을까요?
    fTaskDef(print_A, 20);
    fTaskDef(print_B, 21);
    fTaskDef(print_C, 22);
    fTaskDef(print_D, 22);
 
    fStart();

    Serial.println("Insufficient RAM");
    while (1)
        ;
}
//------------------------------------------------------------------------------
// WARNING idle loop has a very small stack (configMINIMAL_STACK_SIZE)
// loop must never block
void loop() {
    // Not used.
}
