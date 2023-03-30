/////////////////////////////////////////////////////////////////////////////
//
//    fmuSpecific.h
//    fmuSpecific for FMI 2.0
//
//    author: Dassault Syst?mes
//      creation: 2016-12
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __FMU_SPECIFIC_H__
#define __FMU_SPECIFIC_H__

#include "fmuFramework.h"
#include "rte_cfg.h"


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus//

#define MULTI_INSTANTIATION_NOT_ALLOWED 0

//Flashlight_pin_io
#define VRB_Flashlight_pin_io 1
//Flashlight_timer_InputSignal_On
//trigger
#define VRB_Flashlight_timer_InputSignal_On_trigger 2
//Flashlight_timer_InputSignal_Off
//trigger
#define VRB_Flashlight_timer_InputSignal_Off_trigger 3
//Flashlight_light_InputSignal_Tick
//trigger
#define VRB_Flashlight_light_InputSignal_Tick_trigger 4
//Flashlight_pout_io
#define VRB_Flashlight_pout_io 5
//Flashlight_fp_counts_data
#define VRI_Flashlight_fp_counts_data 6
//Flashlight_timer_light_On
//trigger
#define VRB_Flashlight_timer_light_On_trigger 7
//Flashlight_timer_light_Off
//trigger
#define VRB_Flashlight_timer_light_Off_trigger 8
//Flashlight_timer_light_Tick
//trigger
#define VRB_Flashlight_timer_light_Tick_trigger 9
//Flashlight_timer_light_snd
#define VRB_Flashlight_timer_light_snd 10
//Flashlight_timer_control_On
//trigger
#define VRB_Flashlight_timer_control_On_trigger 11
//Flashlight_timer_control_Off
//trigger
#define VRB_Flashlight_timer_control_Off_trigger 12
//Flashlight_timer_control_Tick
//trigger
#define VRB_Flashlight_timer_control_Tick_trigger 13
//Flashlight_timer_control_snd
#define VRB_Flashlight_timer_control_snd 14
//Flashlight_light_light_On
//trigger
#define VRB_Flashlight_light_light_On_trigger 15
//Flashlight_light_light_Off
//trigger
#define VRB_Flashlight_light_light_Off_trigger 16
//Flashlight_light_light_Tick
//trigger
#define VRB_Flashlight_light_light_Tick_trigger 17
//Flashlight_light_light_snd
#define VRB_Flashlight_light_light_snd 18
//Flashlight_light_pout_io
#define VRB_Flashlight_light_pout_io 19
//Flashlight_button_control_On
//trigger
#define VRB_Flashlight_button_control_On_trigger 20
//Flashlight_button_control_Off
//trigger
#define VRB_Flashlight_button_control_Off_trigger 21
//Flashlight_button_control_Tick
//trigger
#define VRB_Flashlight_button_control_Tick_trigger 22
//Flashlight_button_control_snd
#define VRB_Flashlight_button_control_snd 23
//Flashlight_button_pin_io
#define VRB_Flashlight_button_pin_io 24
//Flashlight_count_countin_io
#define VRB_Flashlight_count_countin_io 25
//Flashlight_count_fp_counts_data
#define VRI_Flashlight_count_fp_counts_data 26


typedef struct {
	Boolean Flashlight_pin_io;
	char Flashlight_pin_io_isChanged;
	trigger_type Flashlight_timer_InputSignal_On_trigger;
	char Flashlight_timer_InputSignal_On_trigger_isChanged;
	trigger_type Flashlight_timer_InputSignal_Off_trigger;
	char Flashlight_timer_InputSignal_Off_trigger_isChanged;
	trigger_type Flashlight_light_InputSignal_Tick_trigger;
	char Flashlight_light_InputSignal_Tick_trigger_isChanged;
	Boolean Flashlight_pout_io;
	char Flashlight_pout_io_isChanged;
	Integer Flashlight_fp_counts_data;
	char Flashlight_fp_counts_data_isChanged;
	trigger_type Flashlight_timer_light_On_trigger;
	char Flashlight_timer_light_On_trigger_isChanged;
	trigger_type Flashlight_timer_light_Off_trigger;
	char Flashlight_timer_light_Off_trigger_isChanged;
	trigger_type Flashlight_timer_light_Tick_trigger;
	char Flashlight_timer_light_Tick_trigger_isChanged;
	Boolean Flashlight_timer_light_snd;
	char Flashlight_timer_light_snd_isChanged;
	trigger_type Flashlight_timer_control_On_trigger;
	char Flashlight_timer_control_On_trigger_isChanged;
	trigger_type Flashlight_timer_control_Off_trigger;
	char Flashlight_timer_control_Off_trigger_isChanged;
	trigger_type Flashlight_timer_control_Tick_trigger;
	char Flashlight_timer_control_Tick_trigger_isChanged;
	Boolean Flashlight_timer_control_snd;
	char Flashlight_timer_control_snd_isChanged;
	trigger_type Flashlight_light_light_On_trigger;
	char Flashlight_light_light_On_trigger_isChanged;
	trigger_type Flashlight_light_light_Off_trigger;
	char Flashlight_light_light_Off_trigger_isChanged;
	trigger_type Flashlight_light_light_Tick_trigger;
	char Flashlight_light_light_Tick_trigger_isChanged;
	Boolean Flashlight_light_light_snd;
	char Flashlight_light_light_snd_isChanged;
	Boolean Flashlight_light_pout_io;
	char Flashlight_light_pout_io_isChanged;
	trigger_type Flashlight_button_control_On_trigger;
	char Flashlight_button_control_On_trigger_isChanged;
	trigger_type Flashlight_button_control_Off_trigger;
	char Flashlight_button_control_Off_trigger_isChanged;
	trigger_type Flashlight_button_control_Tick_trigger;
	char Flashlight_button_control_Tick_trigger_isChanged;
	Boolean Flashlight_button_control_snd;
	char Flashlight_button_control_snd_isChanged;
	Boolean Flashlight_button_pin_io;
	char Flashlight_button_pin_io_isChanged;
	Boolean Flashlight_count_countin_io;
	char Flashlight_count_countin_io_isChanged;
	Integer Flashlight_count_fp_counts_data;
	char Flashlight_count_fp_counts_data_isChanged;


} sPrivateCtx;

fmi2Real     getFmuReal     (FMUInstance* comp, const fmi2ValueReference iValueRef);
fmi2Integer  getFmuInteger  (FMUInstance* comp, const fmi2ValueReference iValueRef);
fmi2Boolean  getFmuBoolean  (FMUInstance* comp, const fmi2ValueReference iValueRef);
fmi2String   getFmuString   (FMUInstance* comp, const fmi2ValueReference iValueRef);

void setFmuReal     (FMUInstance* comp, const fmi2ValueReference iValueRef, const fmi2Real fValue);
void setFmuInteger  (FMUInstance* comp, const fmi2ValueReference iValueRef, const fmi2Integer iValue);
void setFmuBoolean  (FMUInstance* comp, const fmi2ValueReference iValueRef, const fmi2Boolean bValue);
void setFmuString   (FMUInstance* comp, const fmi2ValueReference iValueRef, const fmi2String acValue);

void fmuInstanciate		   (FMUInstance* comp);
void fmuInitSpecificContext(FMUInstance* comp);
void fmuInit               (FMUInstance* comp);
void fmuFinalizeInit       (FMUInstance* comp);
void fmuUninit             (FMUInstance* comp);
void fmuStep               (FMUInstance* comp, fmi2Real currentCommunicationPoint, fmi2Real communicationStepSize, fmi2Boolean newStep);

void setFMUInstance(FMUInstance* fmuInstance);
void runScheduler(fmi2Real currentCommunicationPoint, fmi2Real communicationStepSize);
void runPeriodic_comp(task_descriptor* config);

#ifdef __cplusplus
}
#endif // __cplusplus //

#endif // __FMU_SPECIFIC_H__ //
