/////////////////////////////////////////////////////////////////////////////
//
//    fmuSpecific.c
//    fmuSpecific for FMI 2.0
//
//    author: Dassault Syst?mes
//      creation: 2016-12
//
/////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "fmuFramework.h"
#include "fmuSpecific.h"

#ifdef _MSC_VER
	#define strcasecmp  _stricmp
	#define strncasecmp _strnicmp
	#define snprintf    _snprintf
	#define usleep(X)   Sleep(X/1000)
#endif // _MSC_VER //

FMUInstance* fmuInst;

/////////////////////////////////////////////////////////////////////////////
// fmuInitSpecificContext()
/////////////////////////////////////////////////////////////////////////////
void fmuInitSpecificContext(FMUInstance* fmuInstance) {
	if(!fmuInstance->privateCtx) {
		fmuInstance->privateCtx = (void*)malloc(sizeof(sPrivateCtx));
		memset(fmuInstance->privateCtx, 0, sizeof(sPrivateCtx));

	}
}

/////////////////////////////////////////////////////////////////////////////
// getFmu...()
/////////////////////////////////////////////////////////////////////////////
fmi2Real getFmuReal(FMUInstance* fmuInstance, const fmi2ValueReference iValueRef) {
	fmi2Real fValue = 0.0;
	switch(iValueRef) {
		case 0:
			break;
		default:
			if (fmuInstance->debugLoggingOn) {
				fmuInstance->logger(fmuInstance, fmuInstance->instanceName, fmi2Warning, "warning", "Unable to get real: '%d'", iValueRef);
			}
	}
	return fValue;
}

fmi2Integer getFmuInteger(FMUInstance* fmuInstance, const fmi2ValueReference iValueRef) {
	fmi2Integer iValue = 0;
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInstance->privateCtx;
	switch(iValueRef) {
		case VRI_Flashlight_fp_counts_data:
			iValue = pPrivateCtx->Flashlight_fp_counts_data;
			break;
		case VRI_Flashlight_count_fp_counts_data:
			iValue = pPrivateCtx->Flashlight_count_fp_counts_data;
			break;

		default:
			if (fmuInstance->debugLoggingOn) {
				fmuInstance->logger(fmuInstance, fmuInstance->instanceName, fmi2Warning, "warning", "Unable to get integer: '%d'", iValueRef);
			}
	}
	return iValue;
}

fmi2Boolean getFmuBoolean(FMUInstance* fmuInstance, const fmi2ValueReference iValueRef) {
	fmi2Boolean bValue = 0;
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInstance->privateCtx;
	switch(iValueRef) {
		case VRB_Flashlight_pin_io:
			bValue = pPrivateCtx->Flashlight_pin_io;
			break;
		case VRB_Flashlight_timer_InputSignal_On_trigger:
			bValue = pPrivateCtx->Flashlight_timer_InputSignal_On_trigger;
			pPrivateCtx->Flashlight_timer_InputSignal_On_trigger = 0;
			break;
		case VRB_Flashlight_timer_InputSignal_Off_trigger:
			bValue = pPrivateCtx->Flashlight_timer_InputSignal_Off_trigger;
			pPrivateCtx->Flashlight_timer_InputSignal_Off_trigger = 0;
			break;
		case VRB_Flashlight_light_InputSignal_Tick_trigger:
			bValue = pPrivateCtx->Flashlight_light_InputSignal_Tick_trigger;
			pPrivateCtx->Flashlight_light_InputSignal_Tick_trigger = 0;
			break;
		case VRB_Flashlight_pout_io:
			bValue = pPrivateCtx->Flashlight_pout_io;
			break;
		case VRB_Flashlight_timer_light_On_trigger:
			bValue = pPrivateCtx->Flashlight_timer_light_On_trigger;
			pPrivateCtx->Flashlight_timer_light_On_trigger = 0;
			break;
		case VRB_Flashlight_timer_light_Off_trigger:
			bValue = pPrivateCtx->Flashlight_timer_light_Off_trigger;
			pPrivateCtx->Flashlight_timer_light_Off_trigger = 0;
			break;
		case VRB_Flashlight_timer_light_Tick_trigger:
			bValue = pPrivateCtx->Flashlight_timer_light_Tick_trigger;
			pPrivateCtx->Flashlight_timer_light_Tick_trigger = 0;
			break;
		case VRB_Flashlight_timer_light_snd:
			bValue = pPrivateCtx->Flashlight_timer_light_snd;
			break;
		case VRB_Flashlight_timer_control_On_trigger:
			bValue = pPrivateCtx->Flashlight_timer_control_On_trigger;
			pPrivateCtx->Flashlight_timer_control_On_trigger = 0;
			break;
		case VRB_Flashlight_timer_control_Off_trigger:
			bValue = pPrivateCtx->Flashlight_timer_control_Off_trigger;
			pPrivateCtx->Flashlight_timer_control_Off_trigger = 0;
			break;
		case VRB_Flashlight_timer_control_Tick_trigger:
			bValue = pPrivateCtx->Flashlight_timer_control_Tick_trigger;
			pPrivateCtx->Flashlight_timer_control_Tick_trigger = 0;
			break;
		case VRB_Flashlight_timer_control_snd:
			bValue = pPrivateCtx->Flashlight_timer_control_snd;
			break;
		case VRB_Flashlight_light_light_On_trigger:
			bValue = pPrivateCtx->Flashlight_light_light_On_trigger;
			pPrivateCtx->Flashlight_light_light_On_trigger = 0;
			break;
		case VRB_Flashlight_light_light_Off_trigger:
			bValue = pPrivateCtx->Flashlight_light_light_Off_trigger;
			pPrivateCtx->Flashlight_light_light_Off_trigger = 0;
			break;
		case VRB_Flashlight_light_light_Tick_trigger:
			bValue = pPrivateCtx->Flashlight_light_light_Tick_trigger;
			pPrivateCtx->Flashlight_light_light_Tick_trigger = 0;
			break;
		case VRB_Flashlight_light_light_snd:
			bValue = pPrivateCtx->Flashlight_light_light_snd;
			break;
		case VRB_Flashlight_light_pout_io:
			bValue = pPrivateCtx->Flashlight_light_pout_io;
			break;
		case VRB_Flashlight_button_control_On_trigger:
			bValue = pPrivateCtx->Flashlight_button_control_On_trigger;
			pPrivateCtx->Flashlight_button_control_On_trigger = 0;
			break;
		case VRB_Flashlight_button_control_Off_trigger:
			bValue = pPrivateCtx->Flashlight_button_control_Off_trigger;
			pPrivateCtx->Flashlight_button_control_Off_trigger = 0;
			break;
		case VRB_Flashlight_button_control_Tick_trigger:
			bValue = pPrivateCtx->Flashlight_button_control_Tick_trigger;
			pPrivateCtx->Flashlight_button_control_Tick_trigger = 0;
			break;
		case VRB_Flashlight_button_control_snd:
			bValue = pPrivateCtx->Flashlight_button_control_snd;
			break;
		case VRB_Flashlight_button_pin_io:
			bValue = pPrivateCtx->Flashlight_button_pin_io;
			break;
		case VRB_Flashlight_count_countin_io:
			bValue = pPrivateCtx->Flashlight_count_countin_io;
			break;

		default:
			if (fmuInstance->debugLoggingOn) {
				fmuInstance->logger(fmuInstance, fmuInstance->instanceName, fmi2Warning, "warning", "Unable to get boolean: '%d'", iValueRef);
			}
	}
	return bValue;
}

fmi2String getFmuString(FMUInstance* fmuInstance, const fmi2ValueReference iValueRef) {
	fmi2String acValue = "";
	switch(iValueRef) {
		case 0:
			break;
		default:
			if (fmuInstance->debugLoggingOn) {
				fmuInstance->logger(fmuInstance, fmuInstance->instanceName, fmi2Warning, "warning", "Unable to get string: '%d'", iValueRef);
			}
	}
	return acValue;
}

/////////////////////////////////////////////////////////////////////////////
// setFmu...()
/////////////////////////////////////////////////////////////////////////////
void setFmuReal(FMUInstance* fmuInstance, const fmi2ValueReference iValueRef, const fmi2Real fValue) {
	switch(iValueRef) {
		case 0:
			break;
		default:
			if (fmuInstance->debugLoggingOn) {
				fmuInstance->logger(fmuInstance, fmuInstance->instanceName, fmi2Warning, "warning", "Unable to set real: '%d'", iValueRef);
			}
	}
}

void setFmuInteger(FMUInstance* fmuInstance, const fmi2ValueReference iValueRef, const fmi2Integer iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInstance->privateCtx;
	switch(iValueRef) {
		case VRI_Flashlight_fp_counts_data:
			pPrivateCtx->Flashlight_fp_counts_data_isChanged = 1;
			pPrivateCtx->Flashlight_fp_counts_data = iValue;
			break;
		case VRI_Flashlight_count_fp_counts_data:
			pPrivateCtx->Flashlight_count_fp_counts_data_isChanged = 1;
			pPrivateCtx->Flashlight_count_fp_counts_data = iValue;
			break;

		default:
			if (fmuInstance->debugLoggingOn) {
				fmuInstance->logger(fmuInstance, fmuInstance->instanceName, fmi2Warning, "warning", "Unable to set integer: '%d'", iValueRef);
			}
	}
}

void setFmuBoolean(FMUInstance* fmuInstance, const fmi2ValueReference iValueRef, const fmi2Boolean bValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInstance->privateCtx;
	switch(iValueRef) {
		case VRB_Flashlight_pin_io:
			pPrivateCtx->Flashlight_pin_io_isChanged = 1;
			pPrivateCtx->Flashlight_pin_io = bValue;
			break;
		case VRB_Flashlight_timer_InputSignal_On_trigger:
			if(bValue == 1){
				pPrivateCtx->Flashlight_timer_InputSignal_On_trigger_isChanged = 1;
				pPrivateCtx->Flashlight_timer_InputSignal_On_trigger = bValue;
			}
			break;
		case VRB_Flashlight_timer_InputSignal_Off_trigger:
			if(bValue == 1){
				pPrivateCtx->Flashlight_timer_InputSignal_Off_trigger_isChanged = 1;
				pPrivateCtx->Flashlight_timer_InputSignal_Off_trigger = bValue;
			}
			break;
		case VRB_Flashlight_light_InputSignal_Tick_trigger:
			if(bValue == 1){
				pPrivateCtx->Flashlight_light_InputSignal_Tick_trigger_isChanged = 1;
				pPrivateCtx->Flashlight_light_InputSignal_Tick_trigger = bValue;
			}
			break;
		case VRB_Flashlight_pout_io:
			pPrivateCtx->Flashlight_pout_io_isChanged = 1;
			pPrivateCtx->Flashlight_pout_io = bValue;
			break;
		case VRB_Flashlight_timer_light_On_trigger:
			if(bValue == 1){
				pPrivateCtx->Flashlight_timer_light_On_trigger_isChanged = 1;
				pPrivateCtx->Flashlight_timer_light_On_trigger = bValue;
			}
			break;
		case VRB_Flashlight_timer_light_Off_trigger:
			if(bValue == 1){
				pPrivateCtx->Flashlight_timer_light_Off_trigger_isChanged = 1;
				pPrivateCtx->Flashlight_timer_light_Off_trigger = bValue;
			}
			break;
		case VRB_Flashlight_timer_light_Tick_trigger:
			if(bValue == 1){
				pPrivateCtx->Flashlight_timer_light_Tick_trigger_isChanged = 1;
				pPrivateCtx->Flashlight_timer_light_Tick_trigger = bValue;
			}
			break;
		case VRB_Flashlight_timer_light_snd:
			pPrivateCtx->Flashlight_timer_light_snd_isChanged = 1;
			pPrivateCtx->Flashlight_timer_light_snd = bValue;
			break;
		case VRB_Flashlight_timer_control_On_trigger:
			if(bValue == 1){
				pPrivateCtx->Flashlight_timer_control_On_trigger_isChanged = 1;
				pPrivateCtx->Flashlight_timer_control_On_trigger = bValue;
			}
			break;
		case VRB_Flashlight_timer_control_Off_trigger:
			if(bValue == 1){
				pPrivateCtx->Flashlight_timer_control_Off_trigger_isChanged = 1;
				pPrivateCtx->Flashlight_timer_control_Off_trigger = bValue;
			}
			break;
		case VRB_Flashlight_timer_control_Tick_trigger:
			if(bValue == 1){
				pPrivateCtx->Flashlight_timer_control_Tick_trigger_isChanged = 1;
				pPrivateCtx->Flashlight_timer_control_Tick_trigger = bValue;
			}
			break;
		case VRB_Flashlight_timer_control_snd:
			pPrivateCtx->Flashlight_timer_control_snd_isChanged = 1;
			pPrivateCtx->Flashlight_timer_control_snd = bValue;
			break;
		case VRB_Flashlight_light_light_On_trigger:
			if(bValue == 1){
				pPrivateCtx->Flashlight_light_light_On_trigger_isChanged = 1;
				pPrivateCtx->Flashlight_light_light_On_trigger = bValue;
			}
			break;
		case VRB_Flashlight_light_light_Off_trigger:
			if(bValue == 1){
				pPrivateCtx->Flashlight_light_light_Off_trigger_isChanged = 1;
				pPrivateCtx->Flashlight_light_light_Off_trigger = bValue;
			}
			break;
		case VRB_Flashlight_light_light_Tick_trigger:
			if(bValue == 1){
				pPrivateCtx->Flashlight_light_light_Tick_trigger_isChanged = 1;
				pPrivateCtx->Flashlight_light_light_Tick_trigger = bValue;
			}
			break;
		case VRB_Flashlight_light_light_snd:
			pPrivateCtx->Flashlight_light_light_snd_isChanged = 1;
			pPrivateCtx->Flashlight_light_light_snd = bValue;
			break;
		case VRB_Flashlight_light_pout_io:
			pPrivateCtx->Flashlight_light_pout_io_isChanged = 1;
			pPrivateCtx->Flashlight_light_pout_io = bValue;
			break;
		case VRB_Flashlight_button_control_On_trigger:
			if(bValue == 1){
				pPrivateCtx->Flashlight_button_control_On_trigger_isChanged = 1;
				pPrivateCtx->Flashlight_button_control_On_trigger = bValue;
			}
			break;
		case VRB_Flashlight_button_control_Off_trigger:
			if(bValue == 1){
				pPrivateCtx->Flashlight_button_control_Off_trigger_isChanged = 1;
				pPrivateCtx->Flashlight_button_control_Off_trigger = bValue;
			}
			break;
		case VRB_Flashlight_button_control_Tick_trigger:
			if(bValue == 1){
				pPrivateCtx->Flashlight_button_control_Tick_trigger_isChanged = 1;
				pPrivateCtx->Flashlight_button_control_Tick_trigger = bValue;
			}
			break;
		case VRB_Flashlight_button_control_snd:
			pPrivateCtx->Flashlight_button_control_snd_isChanged = 1;
			pPrivateCtx->Flashlight_button_control_snd = bValue;
			break;
		case VRB_Flashlight_button_pin_io:
			pPrivateCtx->Flashlight_button_pin_io_isChanged = 1;
			pPrivateCtx->Flashlight_button_pin_io = bValue;
			break;
		case VRB_Flashlight_count_countin_io:
			pPrivateCtx->Flashlight_count_countin_io_isChanged = 1;
			pPrivateCtx->Flashlight_count_countin_io = bValue;
			break;

		default:
			if (fmuInstance->debugLoggingOn) {
				fmuInstance->logger(fmuInstance, fmuInstance->instanceName, fmi2Warning, "warning", "Unable to set boolean: '%d'", iValueRef);
			}
	}
}

void setFmuString(FMUInstance* fmuInstance, const fmi2ValueReference iValueRef, const fmi2String acValue) {
	switch(iValueRef) {
		case 0:
			break;
		default:
			if (fmuInstance->debugLoggingOn) {
				fmuInstance->logger(fmuInstance, fmuInstance->instanceName, fmi2Warning, "warning", "Unable to set string: '%d'", iValueRef);
			}
	}
}

/////////////////////////////////////////////////////////////////////////////
// buildPayloadFromFMU()
/////////////////////////////////////////////////////////////////////////////
void fmuInstanciate(FMUInstance* fmuInstance) {
	fmuInitSpecificContext(fmuInstance);
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInstance->privateCtx;
	pPrivateCtx->Flashlight_pin_io = 0;
	pPrivateCtx->Flashlight_pin_io_isChanged = 0;
	pPrivateCtx->Flashlight_timer_InputSignal_On_trigger = 0;
	pPrivateCtx->Flashlight_timer_InputSignal_On_trigger_isChanged = 0;
	pPrivateCtx->Flashlight_timer_InputSignal_Off_trigger = 0;
	pPrivateCtx->Flashlight_timer_InputSignal_Off_trigger_isChanged = 0;
	pPrivateCtx->Flashlight_light_InputSignal_Tick_trigger = 0;
	pPrivateCtx->Flashlight_light_InputSignal_Tick_trigger_isChanged = 0;
	pPrivateCtx->Flashlight_pout_io = 0;
	pPrivateCtx->Flashlight_pout_io_isChanged = 0;
	pPrivateCtx->Flashlight_fp_counts_data = 0;
	pPrivateCtx->Flashlight_fp_counts_data_isChanged = 0;
	pPrivateCtx->Flashlight_timer_light_On_trigger = 0;
	pPrivateCtx->Flashlight_timer_light_On_trigger_isChanged = 0;
	pPrivateCtx->Flashlight_timer_light_Off_trigger = 0;
	pPrivateCtx->Flashlight_timer_light_Off_trigger_isChanged = 0;
	pPrivateCtx->Flashlight_timer_light_Tick_trigger = 0;
	pPrivateCtx->Flashlight_timer_light_Tick_trigger_isChanged = 0;
	pPrivateCtx->Flashlight_timer_light_snd = 0;
	pPrivateCtx->Flashlight_timer_light_snd_isChanged = 0;
	pPrivateCtx->Flashlight_timer_control_On_trigger = 0;
	pPrivateCtx->Flashlight_timer_control_On_trigger_isChanged = 0;
	pPrivateCtx->Flashlight_timer_control_Off_trigger = 0;
	pPrivateCtx->Flashlight_timer_control_Off_trigger_isChanged = 0;
	pPrivateCtx->Flashlight_timer_control_Tick_trigger = 0;
	pPrivateCtx->Flashlight_timer_control_Tick_trigger_isChanged = 0;
	pPrivateCtx->Flashlight_timer_control_snd = 0;
	pPrivateCtx->Flashlight_timer_control_snd_isChanged = 0;
	pPrivateCtx->Flashlight_light_light_On_trigger = 0;
	pPrivateCtx->Flashlight_light_light_On_trigger_isChanged = 0;
	pPrivateCtx->Flashlight_light_light_Off_trigger = 0;
	pPrivateCtx->Flashlight_light_light_Off_trigger_isChanged = 0;
	pPrivateCtx->Flashlight_light_light_Tick_trigger = 0;
	pPrivateCtx->Flashlight_light_light_Tick_trigger_isChanged = 0;
	pPrivateCtx->Flashlight_light_light_snd = 0;
	pPrivateCtx->Flashlight_light_light_snd_isChanged = 0;
	pPrivateCtx->Flashlight_light_pout_io = 0;
	pPrivateCtx->Flashlight_light_pout_io_isChanged = 0;
	pPrivateCtx->Flashlight_button_control_On_trigger = 0;
	pPrivateCtx->Flashlight_button_control_On_trigger_isChanged = 0;
	pPrivateCtx->Flashlight_button_control_Off_trigger = 0;
	pPrivateCtx->Flashlight_button_control_Off_trigger_isChanged = 0;
	pPrivateCtx->Flashlight_button_control_Tick_trigger = 0;
	pPrivateCtx->Flashlight_button_control_Tick_trigger_isChanged = 0;
	pPrivateCtx->Flashlight_button_control_snd = 0;
	pPrivateCtx->Flashlight_button_control_snd_isChanged = 0;
	pPrivateCtx->Flashlight_button_pin_io = 0;
	pPrivateCtx->Flashlight_button_pin_io_isChanged = 0;
	pPrivateCtx->Flashlight_count_countin_io = 0;
	pPrivateCtx->Flashlight_count_countin_io_isChanged = 0;
	pPrivateCtx->Flashlight_count_fp_counts_data = 0;
	pPrivateCtx->Flashlight_count_fp_counts_data_isChanged = 0;

}

void fmuInit(FMUInstance* fmuInstance) {
	component_instances_init();
	task_descriptors_init();
	components_init();
}

void fmuFinalizeInit(FMUInstance* fmuInstance) {
}

void fmuStep(FMUInstance* fmuInstance, fmi2Real currentCommunicationPoint, fmi2Real communicationStepSize, fmi2Boolean newStep) {
	setFMUInstance(fmuInstance);
	runScheduler(currentCommunicationPoint, communicationStepSize);
}

void fmuUninit(FMUInstance* fmuInstance) {

}

void setFMUInstance(FMUInstance* fmuInstance){
	fmuInst = fmuInstance;
}

void runScheduler(fmi2Real currentSec, fmi2Real stepSizeSec){
	long long currentMilliSec = currentSec * 1000;
	long long stepSizeMilliSec = stepSizeSec * 1000;
	long long stepMilliSec = currentMilliSec;
	while (stepMilliSec < currentMilliSec + stepSizeMilliSec) {
		for (int i = 0; i < NUMBER_OF_TASKS; i++){
			task_descriptor* config = &task_descriptors[i];
			if (config->type == PERIODIC) {
				if (stepMilliSec % config->period == 0) {
					runPeriodic_comp(config);
				}
			}
		}
		stepMilliSec++;
	}
}

void runPeriodic_comp(task_descriptor* config) {
	int i = 0;
	for (i = 0; i < config->nfunctions; i++) {
		config->functions[i]((void*) (config->rte_instances[i]));
	}
}

void RTE_ReadHook_Flashlight_timer_InputSignal_On_Start(SignalEvent_On_type* iValue) {
}

void RTE_ReadHook_Flashlight_timer_InputSignal_On_Return(SignalEvent_On_type* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	if(pPrivateCtx->Flashlight_timer_InputSignal_On_trigger_isChanged == 1){
		(*iValue).trigger = pPrivateCtx->Flashlight_timer_InputSignal_On_trigger;
		pPrivateCtx->Flashlight_timer_InputSignal_On_trigger_isChanged = 0;
	}
}

void RTE_ReadHook_Flashlight_timer_InputSignal_Off_Start(SignalEvent_Off_type* iValue) {
}

void RTE_ReadHook_Flashlight_timer_InputSignal_Off_Return(SignalEvent_Off_type* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	if(pPrivateCtx->Flashlight_timer_InputSignal_Off_trigger_isChanged == 1){
		(*iValue).trigger = pPrivateCtx->Flashlight_timer_InputSignal_Off_trigger;
		pPrivateCtx->Flashlight_timer_InputSignal_Off_trigger_isChanged = 0;
	}
}

void RTE_ReadHook_Flashlight_light_InputSignal_Tick_Start(SignalEvent_Tick_type* iValue) {
}

void RTE_ReadHook_Flashlight_light_InputSignal_Tick_Return(SignalEvent_Tick_type* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	if(pPrivateCtx->Flashlight_light_InputSignal_Tick_trigger_isChanged == 1){
		(*iValue).trigger = pPrivateCtx->Flashlight_light_InputSignal_Tick_trigger;
		pPrivateCtx->Flashlight_light_InputSignal_Tick_trigger_isChanged = 0;
	}
}

void RTE_TriggerHook_Flashlight_timer_light_On_Start(SignalEvent_On_type* iValue) {
}

void RTE_TriggerHook_Flashlight_timer_light_On_Return(SignalEvent_On_type* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	pPrivateCtx->Flashlight_timer_light_On_trigger = (*iValue).trigger;
}

void RTE_TriggerHook_Flashlight_timer_light_Off_Start(SignalEvent_Off_type* iValue) {
}

void RTE_TriggerHook_Flashlight_timer_light_Off_Return(SignalEvent_Off_type* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	pPrivateCtx->Flashlight_timer_light_Off_trigger = (*iValue).trigger;
}

void RTE_TriggerHook_Flashlight_timer_light_Tick_Start(SignalEvent_Tick_type* iValue) {
}

void RTE_TriggerHook_Flashlight_timer_light_Tick_Return(SignalEvent_Tick_type* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	pPrivateCtx->Flashlight_timer_light_Tick_trigger = (*iValue).trigger;
}

void RTE_WriteHook_Flashlight_timer_light_snd_Start(Boolean* iValue) {
}

void RTE_WriteHook_Flashlight_timer_light_snd_Return(Boolean* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	pPrivateCtx->Flashlight_timer_light_snd = (*iValue);
}

void RTE_ReadHook_Flashlight_timer_control_On_Start(SignalEvent_On_type* iValue) {
}

void RTE_ReadHook_Flashlight_timer_control_On_Return(SignalEvent_On_type* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	if(pPrivateCtx->Flashlight_timer_control_On_trigger_isChanged == 1){
		(*iValue).trigger = pPrivateCtx->Flashlight_timer_control_On_trigger;
		pPrivateCtx->Flashlight_timer_control_On_trigger_isChanged = 0;
	}
}

void RTE_ReadHook_Flashlight_timer_control_Off_Start(SignalEvent_Off_type* iValue) {
}

void RTE_ReadHook_Flashlight_timer_control_Off_Return(SignalEvent_Off_type* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	if(pPrivateCtx->Flashlight_timer_control_Off_trigger_isChanged == 1){
		(*iValue).trigger = pPrivateCtx->Flashlight_timer_control_Off_trigger;
		pPrivateCtx->Flashlight_timer_control_Off_trigger_isChanged = 0;
	}
}

void RTE_ReadHook_Flashlight_timer_control_Tick_Start(SignalEvent_Tick_type* iValue) {
}

void RTE_ReadHook_Flashlight_timer_control_Tick_Return(SignalEvent_Tick_type* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	if(pPrivateCtx->Flashlight_timer_control_Tick_trigger_isChanged == 1){
		(*iValue).trigger = pPrivateCtx->Flashlight_timer_control_Tick_trigger;
		pPrivateCtx->Flashlight_timer_control_Tick_trigger_isChanged = 0;
	}
}

void RTE_ReadHook_Flashlight_timer_control_snd_Start(Boolean* iValue) {
}

void RTE_ReadHook_Flashlight_timer_control_snd_Return(Boolean* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	if(pPrivateCtx->Flashlight_timer_control_snd_isChanged == 1){
		(*iValue) = pPrivateCtx->Flashlight_timer_control_snd;
		pPrivateCtx->Flashlight_timer_control_snd_isChanged = 0;
	}
}

void RTE_ReadHook_Flashlight_light_light_On_Start(SignalEvent_On_type* iValue) {
}

void RTE_ReadHook_Flashlight_light_light_On_Return(SignalEvent_On_type* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	if(pPrivateCtx->Flashlight_light_light_On_trigger_isChanged == 1){
		(*iValue).trigger = pPrivateCtx->Flashlight_light_light_On_trigger;
		pPrivateCtx->Flashlight_light_light_On_trigger_isChanged = 0;
	}
}

void RTE_ReadHook_Flashlight_light_light_Off_Start(SignalEvent_Off_type* iValue) {
}

void RTE_ReadHook_Flashlight_light_light_Off_Return(SignalEvent_Off_type* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	if(pPrivateCtx->Flashlight_light_light_Off_trigger_isChanged == 1){
		(*iValue).trigger = pPrivateCtx->Flashlight_light_light_Off_trigger;
		pPrivateCtx->Flashlight_light_light_Off_trigger_isChanged = 0;
	}
}

void RTE_ReadHook_Flashlight_light_light_Tick_Start(SignalEvent_Tick_type* iValue) {
}

void RTE_ReadHook_Flashlight_light_light_Tick_Return(SignalEvent_Tick_type* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	if(pPrivateCtx->Flashlight_light_light_Tick_trigger_isChanged == 1){
		(*iValue).trigger = pPrivateCtx->Flashlight_light_light_Tick_trigger;
		pPrivateCtx->Flashlight_light_light_Tick_trigger_isChanged = 0;
	}
}

void RTE_ReadHook_Flashlight_light_light_snd_Start(Boolean* iValue) {
}

void RTE_ReadHook_Flashlight_light_light_snd_Return(Boolean* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	if(pPrivateCtx->Flashlight_light_light_snd_isChanged == 1){
		(*iValue) = pPrivateCtx->Flashlight_light_light_snd;
		pPrivateCtx->Flashlight_light_light_snd_isChanged = 0;
	}
}

void RTE_WriteHook_Flashlight_light_pout_io_Start(Boolean* iValue) {
}

void RTE_WriteHook_Flashlight_light_pout_io_Return(Boolean* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	pPrivateCtx->Flashlight_light_pout_io = (*iValue);
	pPrivateCtx->Flashlight_pout_io = (*iValue);
}

void RTE_TriggerHook_Flashlight_button_control_On_Start(SignalEvent_On_type* iValue) {
}

void RTE_TriggerHook_Flashlight_button_control_On_Return(SignalEvent_On_type* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	pPrivateCtx->Flashlight_button_control_On_trigger = (*iValue).trigger;
}

void RTE_TriggerHook_Flashlight_button_control_Off_Start(SignalEvent_Off_type* iValue) {
}

void RTE_TriggerHook_Flashlight_button_control_Off_Return(SignalEvent_Off_type* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	pPrivateCtx->Flashlight_button_control_Off_trigger = (*iValue).trigger;
}

void RTE_TriggerHook_Flashlight_button_control_Tick_Start(SignalEvent_Tick_type* iValue) {
}

void RTE_TriggerHook_Flashlight_button_control_Tick_Return(SignalEvent_Tick_type* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	pPrivateCtx->Flashlight_button_control_Tick_trigger = (*iValue).trigger;
}

void RTE_WriteHook_Flashlight_button_control_snd_Start(Boolean* iValue) {
}

void RTE_WriteHook_Flashlight_button_control_snd_Return(Boolean* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	pPrivateCtx->Flashlight_button_control_snd = (*iValue);
}

void RTE_ReadHook_Flashlight_button_pin_io_Start(Boolean* iValue) {
}

void RTE_ReadHook_Flashlight_button_pin_io_Return(Boolean* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	if(pPrivateCtx->Flashlight_button_pin_io_isChanged == 1){
		(*iValue) = pPrivateCtx->Flashlight_button_pin_io;
		pPrivateCtx->Flashlight_button_pin_io_isChanged = 0;
	}
	if(pPrivateCtx->Flashlight_pin_io_isChanged == 1){
		(*iValue) = pPrivateCtx->Flashlight_pin_io;
		pPrivateCtx->Flashlight_pin_io_isChanged = 0;
	}
}

void RTE_ReadHook_Flashlight_count_countin_io_Start(Boolean* iValue) {
}

void RTE_ReadHook_Flashlight_count_countin_io_Return(Boolean* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	if(pPrivateCtx->Flashlight_count_countin_io_isChanged == 1){
		(*iValue) = pPrivateCtx->Flashlight_count_countin_io;
		pPrivateCtx->Flashlight_count_countin_io_isChanged = 0;
	}
}

void RTE_WriteHook_Flashlight_count_fp_counts_data_Start(Integer* iValue) {
}

void RTE_WriteHook_Flashlight_count_fp_counts_data_Return(Integer* iValue) {
	sPrivateCtx* pPrivateCtx = (sPrivateCtx*)fmuInst->privateCtx;
	pPrivateCtx->Flashlight_count_fp_counts_data = (*iValue);
	pPrivateCtx->Flashlight_fp_counts_data = (*iValue);
}


