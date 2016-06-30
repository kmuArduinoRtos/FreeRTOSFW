/*
    KMU ARDUINO RTOS - Copyright (C) 2016 KOOKMIN UNIV CS.
    All rights reserved

    VISIT https://github.com/kmuArduinoRtos/FreeRTOSFW/tree/master/FreeRTOS_AVR/src/utility 
    TO ENSURE YOU ARE USING THE LATEST VERSION.
*/

#ifndef FRAME_WORK_H
#define FRAME_WORK_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void * TaskHandle_t;

/* 태스크의 우선순위 설정을 위해 사용됩니다.
@TaskHandle_t handler: 태스크 핸들러
@long period: 태스크 주기*/
typedef struct FxReadyTask
{
TaskHandle_t handler;
long period;
} FReadyTask;

/* 태스크의 주기 설정과 함수 코드 간략화를 위해 사용됩니다.
@void (*funcPtr)(): 사용자 정의 함수의 함수 포인터
@long period: 태스크 주기*/
typedef struct FxHideParams
{
void (*funcPtr)();
long period;
} FHideParams;

/*프레임워크 계층의 태스크를 정의하는 함수입니다.
@Param funcPtr: 사용자 정의 함수의 함수 포인터
@Param long: 사용자가 명시한 태스크의 주기*/
BaseType_t fTaskDef( void (*funcPtr)() , long ) PRIVILEGED_FUNCTION ;

/*프레임워크 계층의 스케쥴러 호출 함수입니다.*/
void fInitTasks( void ) PRIVILEGED_FUNCTION;

/*정렬 함수 입니다. Bubble Sort.*/
void fSort() PRIVILEGED_FUNCTION;

/*프레임워크 계층의 딜레이 함수입니다.
@Param long: 딜레이 시간(ms)*/
void fDelay(long) PRIVILEGED_FUNCTION;

/* = static void fHideFunc(void*) =
FreeRTOS 계층의 실제 태스크가 되는 함수입니다.
@Param (FHideParams*)void* 사용자가 정의한 함수의 함수포인터와 태스크의 주기 */
static void fHideFunc(void*);

#ifdef __cplusplus
}
#endif
#endif