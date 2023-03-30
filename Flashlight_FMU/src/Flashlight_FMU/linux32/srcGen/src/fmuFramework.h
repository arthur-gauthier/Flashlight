/////////////////////////////////////////////////////////////////////////////
//
//    fmuFramework.h
//    fmuFramework for FMI 2.0
//
//    author: Dassault Syst?mes
//      creation: 2016-12
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __FMU_FRAMEWORK_H__
#define __FMU_FRAMEWORK_H__


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus //

#include "fmi2Functions.h"


#define not_fmuError (fmuInstantiated|fmuInitializing|fmuInitialized|fmuTerminated)

typedef enum {
	fmuInstantiated = 1<<0,
	fmuInitializing = 1<<1,
	fmuInitialized  = 1<<2,
	fmuTerminated   = 1<<3,
	fmuError        = 1<<4
} fmuStatus;


typedef struct {
	fmi2Real                   time;
	fmi2String                 instanceName;
	fmi2String                 fmuLocation;
	fmi2CallbackLogger         logger;
	fmi2CallbackAllocateMemory allocateMemory;
	fmi2CallbackFreeMemory     freeMemory;
	fmi2StepFinished           stepFinished;
	fmi2ComponentEnvironment   componentEnvironment;
	fmi2Boolean                debugLoggingOn;
	fmuStatus                  status;
	void*                      privateCtx;
} FMUInstance;

void  fmu_enterCriticalSection(FMUInstance* comp);
void  fmu_exitCriticalSection (FMUInstance* comp);
char* getFilePath(FMUInstance* comp, const char* acFilename);

#ifdef __cplusplus
}
#endif // __cplusplus //

#endif // __FMU_FRAMEWORK_H__ //
