#include "FreeRTOS.h"
#include "task.h"
#include "framework.h"


#define configMAX_NUMBER_OF_TASKS 4

PRIVILEGED_DATA static UBaseType_t fNumberOfTasks = ( UBaseType_t ) 0U;
PRIVILEGED_DATA static FReadyTask fReadyTasks[configMAX_NUMBER_OF_TASKS];
PRIVILEGED_DATA static FHideParams fHideTasks[configMAX_NUMBER_OF_TASKS];


static void fHideFunc(void* fHide){
	void (*funcPtr)();
	long period;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	funcPtr = ((FHideParams*)fHide)->funcPtr;
	period = ((FHideParams*)fHide)->period;
	while(1){
		funcPtr();
		vTaskDelayUntil( &xLastWakeTime, ( period / portTICK_PERIOD_MS ) );  
	}
}


BaseType_t fTaskDef(void (*funcPtr)(), long fPeriod){
	BaseType_t fReturn;
	TaskHandle_t fTaskHandle;

	fHideTasks[fNumberOfTasks].period = fPeriod;
	fHideTasks[fNumberOfTasks].funcPtr = funcPtr;

	fReadyTasks[fNumberOfTasks].handler = fTaskHandle;
	fReturn = xTaskCreate(fHideFunc, NULL, configMINIMAL_STACK_SIZE, (void*)&(fHideTasks[fNumberOfTasks]), 1, &fReadyTasks[fNumberOfTasks].handler);
	fReadyTasks[fNumberOfTasks].period = fPeriod;

	fNumberOfTasks++;
	
	return fReturn;
}

void fInitTasks( void ){
	UBaseType_t fIndex, fPriority;
	fIndex = ( UBaseType_t ) 1U;
	fPriority =  ( UBaseType_t )1U;
	fSort();

	vTaskStartScheduler();
}





void fDelay( long timeDelay ){
	vTaskDelay((timeDelay * configTICK_RATE_HZ) / 1000L);
}

UBaseType_t getfNumberOfTasks(){
	return fNumberOfTasks;
}

FReadyTask getfReadyTasks(){
	return fReadyTasks[configMAX_NUMBER_OF_TASKS-1];
}

UBaseType_t getone(){
	return 1;
}

void fSort()
{

  UBaseType_t c, d;
  FReadyTask t;
 
  for (c = ( UBaseType_t )0U ; c <fNumberOfTasks - 1; c++)
  {
    for (d = ( UBaseType_t )0U ; d < fNumberOfTasks - c - 1; d++)
    {
      if (fReadyTasks[d].period < fReadyTasks[d+1].period){
        /* Swapping */

               t         = fReadyTasks[d];
        fReadyTasks[d]   = fReadyTasks[d+1];
        fReadyTasks[d+1] = t;
      }
    }
  }
}