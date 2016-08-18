/*
    RTOS Framework for Arduino
    Copyright (c) 2016, School of Computer Enge. Kookmin Univ., Seoul, Korea
    All rights reserved

    VISIT https://github.com/kmuArduinoRtos/FreeRTOSFW/tree/master/FreeRTOS_AVR/src/utility 
    TO ENSURE YOU ARE USING THE LATEST VERSION.
    // MINSUK --> framwework.h 코멘트 참조.
*/

/* FrameWork includes. */
#include "kmu_framework.h"

/* 생성될 수 있는 최대 태스크의 개수입니다. 이 값은 FreeRTOSConfig.h의 configMAX_PRIORITIES와 관련되어 있습니다. 
만약 configMAX_PRIORITIES 값이 4라면, 우선순위는 IdleTask를 위한 0을 제외하고 1~4가 각각의 태스크에 할당됩니다. 
configMAX_PRIORITIES과 마찬가지로 이 값은 사용자에 맞게 수정하면 됩니다. */
#define configMAX_NUMBER_OF_TASKS 4

/*-----------------------------------------------------------*/

/* 이 스태틱 변수들은 생성된 태스크의 정보를 저장하는데 사용됩니다. */
/* fNumberOfTasks는 현재 생성된 태스크의 개수 입니다.*/    
PRIVILEGED_DATA static UBaseType_t fNumberOfTasks = ( UBaseType_t ) 0U;

/* fReadyTasks, fHideTasks에 관한 자세한 사항은 framework.h를 참조 하십시오. */
PRIVILEGED_DATA static FReadyTask fReadyTasks[configMAX_NUMBER_OF_TASKS];
PRIVILEGED_DATA static FHideParams fHideTasks[configMAX_NUMBER_OF_TASKS];

/* fHideFunc는 FreeRTOS xTaskCreate()의 실제 대상이 되는 함수입니다.
사용자가 정의한 함수는 이 함수에 포함되어 실행되며, 사용자가 명시한 태스크의 주기도 이 함수에서 실행됩니다. 
또한, FreeRTOS의 함수에서 필요하였던 while()도 이 함수에 포함되어 있습니다. */
static void fHideFunc(void* fHide){
    void (*funcPtr)();
    long period;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    funcPtr = ((FHideParams*)fHide)->funcPtr;
    period = ((FHideParams*)fHide)->period;

    while(1)
    {
        funcPtr();
        vTaskDelayUntil( &xLastWakeTime, ( period / portTICK_PERIOD_MS ) );  
    }
}

/* fTaskDef()는 프레임워크에서 사용되는 스케쥴러 호출 함수 입니다.
fTaskDef()가 호출 되면 fHideFunc 태스크가 생성됩니다. 
이 때, 사용자가 정의한 함수와 주기는 fHideFunc() 함수의 파라미터가 됩니다.*/
BaseType_t fTaskDef(void (*funcPtr)(), long fPeriod){
    BaseType_t fReturn;
    TaskHandle_t fTaskHandle;

//    debug_print();

//debug_print_line_number(__func__,__LINE__);
    /* fHideFunc() 함수의 파라미터로 사용하기 위해 함수와 주기 값을 fHideTasks 구조체에 복사합니다.*/
    fHideTasks[fNumberOfTasks].period = fPeriod;
    fHideTasks[fNumberOfTasks].funcPtr = funcPtr;

    /* 태스크의 핸들러와 주기를 fReadyTasks에 저장합니다.
    이 값은 fInitTasks()에서 각 태스크의 우선순위값을 설정할 때 사용합니다. */
    fReadyTasks[fNumberOfTasks].handler = fTaskHandle;
    fReadyTasks[fNumberOfTasks].period = fPeriod;

    /* FreeRTOS 계층의 태스크를 생성합니다. 실제 생성되는 태스크는 fHideFunc이며,
       파라미터는 사용자가 정의한 함수와 주기이며, 우선순위는 1로 초기화 합니다. 
       각 태스크의 우선순위는 fInitTasks()가 호출 되면 설정됩니다.
       이를 위해서는 vTaskPrioritySet() 호출해야 하는데, 이 때 핸들러가 필요합니다.
       태스크의 이름, 스택크기는 기본값을 갖습니다. */
    fReturn = xTaskCreate(fHideFunc, NULL, configMINIMAL_STACK_SIZE, (void*)&(fHideTasks[fNumberOfTasks]), 1, &fReadyTasks[fNumberOfTasks].handler);

//debug_print_line_number(__func__,__LINE__);
    fNumberOfTasks++;
    return fReturn;
}

/* 프레임워크 계층의 스케쥴러 실행 함수입니다. FreeRTOS 계층의 vTaskStartScheduler()를 호출하는데,
그 전에 vTaskPrioritySet()를 이용해 각 태스크에 우선순위를 부여합니다. 
큰 주기를 갖은 태스크는 낮은 우선 순위를 갖게 됩니다.*/
static void fInitTasks(void *arg){
    UBaseType_t fIndex, fPriority;
    fIndex = ( UBaseType_t ) 0U;
    fPriority =  fIndex + 1;
    fSort();

//    debug_print_line_number(__func__,__LINE__);
    while(fNumberOfTasks > fIndex)
    {
        if(fIndex > 0)
        {
            if(fReadyTasks[fIndex-1].period != fReadyTasks[fIndex].period)
            {
                fPriority++;
            }
        }
        vTaskPrioritySet(fReadyTasks[fIndex].handler, tskIDLE_PRIORITY + fPriority);
        fIndex++;
    }		
    vTaskDelete(NULL);
}

/* 프레임워크 계층의 태스크의 딜레이 함수 입니다. 사용자 함수에서 사용 됩니다.*/
void fDelay( long timeDelay )
{
    vTaskDelay((timeDelay * configTICK_RATE_HZ) / 1000L);
}

void fStart()
{
//debug_print_line_number(__func__,__LINE__);
    xTaskCreate(fInitTasks, NULL, configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
    vTaskStartScheduler();

//debug_print_line_number(__func__,__LINE__);
}

/* 일반적인 정렬 함수 입니다. 태스크를 주기에 따라 정렬합니다.*/
void fSort()
{
    UBaseType_t c, d;
    FReadyTask t;
    for (c = ( UBaseType_t )0U ; c <fNumberOfTasks - 1; c++)
    {
        for (d = ( UBaseType_t )0U ; d < fNumberOfTasks - c - 1; d++)
        {
            if (fReadyTasks[d].period < fReadyTasks[d+1].period)
            {
                t                = fReadyTasks[d];
                fReadyTasks[d]   = fReadyTasks[d+1];
                fReadyTasks[d+1] = t;
            }
        }
    }
}
