#ifndef FRAME_WORK_H
#define FRAME_WORK_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void * TaskHandle_t;

BaseType_t fTaskDef( void (*funcPtr)() , long ) PRIVILEGED_FUNCTION ;
void fInitTasks( void ) PRIVILEGED_FUNCTION;
void fSort() PRIVILEGED_FUNCTION;
void fDelay(long) PRIVILEGED_FUNCTION;


static void fHideFunc(void*);

typedef struct FxReadyTask
{
TaskHandle_t handler;
long period;
} FReadyTask;

typedef struct FxHideParams
{
void (*funcPtr)();
long period;
} FHideParams;

UBaseType_t getfNumberOfTasks() PRIVILEGED_FUNCTION;
FReadyTask getfReadyTasks() PRIVILEGED_FUNCTION;
FReadyTask getOne() PRIVILEGED_FUNCTION;

#ifdef __cplusplus
}
#endif
#endif