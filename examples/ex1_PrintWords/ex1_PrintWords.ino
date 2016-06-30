
/*arduino due,...(ARM)*/
#include <FreeRTOS_ARM.h>

/*arduino uno,...(AVR)*/
//#include <FreeRTOS_AVR.h>

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
  
  /*If your code are normal, program will not arrive here. */
  Serial.println("Insufficient RAM");
  while(1);
}

/*Don`t touch here. This section is not needed in FreeRTOS. */
void loop() {
}
