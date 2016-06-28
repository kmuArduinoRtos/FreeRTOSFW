#ifndef FRAME_WORK_H
#define FRAME_WORK_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void * TaskHandle_t;

BaseType_t fTaskDef( TaskFunction_t , UBaseType_t ) PRIVILEGED_FUNCTION ;
void fInitTasks( void ) PRIVILEGED_FUNCTION;
void fSort() PRIVILEGED_FUNCTION;

typedef struct fxReadyTask
{
	TaskHandle_t handler;
	UBaseType_t period;
} fReadyTask;
#ifdef __cplusplus
}
#endif
#endif