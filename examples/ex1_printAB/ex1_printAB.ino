#include <FreeRTOS_ARM.h>

void TaskA() {
    Serial.print("A");
}
//------------------------------------------------------------------------------
void TaskB() {
    Serial.print("B");
}
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  fTaskDef(TaskA, 1000);
  fTaskDef(TaskB, 2000);
  fInitTasks();
  
  Serial.println("Insufficient RAM");
  while(1);
}
void loop() {
}
