/*2023-03-30T15:07:49.291214849Z*/

/**********************************************************************
 * Timer_itf.h
 * Header File containing the definition of component: Timer
 *
 * generated by : embedded software producer / runtime code gen 1.0.0
 **********************************************************************/

#ifndef RTE_Timer_ITF_H
#define RTE_Timer_ITF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* Includes -------------------------------------*/
#include "Timer_type.h"

// Workaround: define a default value for instances if not declared outside but it should not be here
#ifndef CONTRACT_VALIDATION
#include "rte_cfg.h" // define NB_INST_TIMER
#else
#define NB_INST_TIMER 1
#endif

/* RTE Component Structure declaration ----------*/
typedef struct{
	/* Write functions */
	void (*Write_light_snd)(Boolean* msg);
	/* Trigger functions */
	void (*Trigger_light_On)(void);
	void (*Trigger_light_Off)(void);
	void (*Trigger_light_Tick)(void);
	/* Read functions */
	void (*Read_control_snd)(Boolean* msg);
	void (*Read_control_On)(SignalEvent_On_type* msg);
	void (*Read_control_Off)(SignalEvent_Off_type* msg);
	void (*Read_control_Tick)(SignalEvent_Tick_type* msg);
	void (*Read_InputSignal_On)(SignalEvent_On_type* msg);
	void (*Read_InputSignal_Off)(SignalEvent_Off_type* msg);
	/* Server call functions */
	/* Activation period for components cyclically invoked, -1 otherwise */
	double period;
	/* pointer to user section */
	void* user_section;
}Rte_Timer_Struct;

/* Runnable functions declaration--------------- */
void Timer_Timer(Rte_Timer_Struct* instance);
void Timer_Timer_init(Rte_Timer_Struct* instance);

#ifndef RTE_C

/* Runnable API Mapping -------------------------*/
#define Timer Timer_Timer
#define Timer_init Timer_Timer_init

/* Instance handle type -------------------------*/
typedef Rte_Timer_Struct* /*const*/ Rte_Instance;

#ifdef RTE_CONTRACT
/* RTE Contract For Documentation Purpose --------*/
/* Write functions declaration */
void RTE_Write_light_snd(Rte_Instance instance, Boolean* msg);
/* Trigger functions declaration */
void RTE_Trigger_light_On(Rte_Instance instance);
void RTE_Trigger_light_Off(Rte_Instance instance);
void RTE_Trigger_light_Tick(Rte_Instance instance);
/* Read functions declaration*/
void RTE_Read_control_snd(Rte_Instance instance, Boolean* msg);
void RTE_Read_control_On(Rte_Instance instance, SignalEvent_On_type* msg);
void RTE_Read_control_Off(Rte_Instance instance, SignalEvent_Off_type* msg);
void RTE_Read_control_Tick(Rte_Instance instance, SignalEvent_Tick_type* msg);
void RTE_Read_InputSignal_On(Rte_Instance instance, SignalEvent_On_type* msg);
void RTE_Read_InputSignal_Off(Rte_Instance instance, SignalEvent_Off_type* msg);
/* Server call functions declaration*/

#else

/* RTE API Mapping ------------------------------*/
#define RTE_Write_light_snd(inst, data) (inst)->Write_light_snd(data)
#define RTE_Trigger_light_On(inst) (inst)->Trigger_light_On()
#define RTE_Trigger_light_Off(inst) (inst)->Trigger_light_Off()
#define RTE_Trigger_light_Tick(inst) (inst)->Trigger_light_Tick()
#define RTE_Read_control_snd(inst, data) (inst)->Read_control_snd(data)
#define RTE_Read_control_On(inst, data) (inst)->Read_control_On(data)
#define RTE_Read_control_Off(inst, data) (inst)->Read_control_Off(data)
#define RTE_Read_control_Tick(inst, data) (inst)->Read_control_Tick(data)
#define RTE_Read_InputSignal_On(inst, data) (inst)->Read_InputSignal_On(data)
#define RTE_Read_InputSignal_Off(inst, data) (inst)->Read_InputSignal_Off(data)
#endif

#endif /* #ifndef RTE_C*/

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* Timer_ITF_H */