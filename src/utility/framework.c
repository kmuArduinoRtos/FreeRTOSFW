#include "FreeRTOS.h"
#include "task.h"
#include "framework.h"


#define configMAX_NUMBER_OF_TASKS 4

PRIVILEGED_DATA static UBaseType_t fNumberOfTasks = ( UBaseType_t ) 0U;
PRIVILEGED_DATA static fReadyTask fReadyTasks[configMAX_NUMBER_OF_TASKS];


BaseType_t fTaskDef(TaskFunction_t pxTaskCode, UBaseType_t fPeriod){
	BaseType_t fReturn;
	TaskHandle_t fTaskHandle;
	fReadyTasks[fNumberOfTasks].handler = fTaskHandle;

	fReturn = xTaskCreate(pxTaskCode, NULL, configMINIMAL_STACK_SIZE, NULL, 1, &fReadyTasks[fNumberOfTasks].handler);

	fReadyTasks[fNumberOfTasks].period = fPeriod;
	fNumberOfTasks++;
	
	return fReturn;
}





void fInitTasks( void ){
			UBaseType_t fIndex, fPriority;
			fPriority = ( UBaseType_t ) 1U;
			fIndex = ( UBaseType_t ) 0U;
						
			fSort();

			while(fNumberOfTasks > fIndex){

					vTaskPrioritySet(fReadyTasks[fIndex].handler, tskIDLE_PRIORITY + configMAX_PRIORITIES - fPriority++);
					fPriority++;
					fIndex++;					

			}		
			 
			vTaskStartScheduler();
}

void fSort()
{

  UBaseType_t c, d;
  fReadyTask t;
 
  for (c = ( UBaseType_t )0U ; c < ( fNumberOfTasks - 1 ); c++)
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
