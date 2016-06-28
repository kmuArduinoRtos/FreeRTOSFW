/*
 * Example to demonstrate thread definition, semaphores, and thread sleep.
 */
#include <FreeRTOS_AVR.h>

// The LED is attached to pin 13 on Arduino.
//const uint8_t LED_PIN = 13;

// Declare a semaphore handle.
//SemaphoreHandle_t sem;
//------------------------------------------------------------------------------

// Declare the thread function for thread 1.
static void Thread1(void* arg) {
  long timeDelay = 400;
  while (1) {
    Serial.print("A");
  }
}
//------------------------------------------------------------------------------

// Declare the thread function for thread 2.
static void Thread2(void* arg) {
  long timeDelay = 400;
  while (1) {
    Serial.print("B");
  }
}

static void Thread3(void* arg) {
  long timeDelay = 400;
  while (1) {
    Serial.print("C");
  }
}

static void Thread4(void* arg) {
  long timeDelay = 400;
  while (1) {
    Serial.print("D");
  }
}
//------------------------------------------------------------------------------
void setup() {
  portBASE_TYPE s1, s2;

  Serial.begin(9600);
  
  //int priorityL = 1;
  
  int priorityA = 1;
  int priorityB = 2;
  // initialize semaphore
  //sem = xSemaphoreCreateCounting(1, 0);
  //s0 = xTaskCreate(ThreadL, NULL, configMINIMAL_STACK_SIZE, NULL, priorityL, NULL);

  fTaskDef(Thread1, 2);
  fTaskDef(Thread2, 5);
  fTaskDef(Thread3, 7);
  //fTaskDef(Thread3, 30);
  //xTaskCreate(Thread3, NULL, configMINIMAL_STACK_SIZE, NULL, 3, NULL);
  //xTaskCreate(Thread2, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);
  fInitTasks();
  
  

  // check for creation errors
  /*if (sem== NULL || s1 != pdPASS || s2 != pdPASS ) {
    Serial.println(F("Creation problem"));
    while(1);
  }*/
  // start scheduler
  
  Serial.println("Insufficient RAM");
  while(1);
}
//------------------------------------------------------------------------------
// WARNING idle loop has a very small stack (configMINIMAL_STACK_SIZE)
// loop must never block
void loop() {
  // Not used.
}
