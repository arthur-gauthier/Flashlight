/*2023-03-30T15:08:15.059752247Z*/

/**********************************************************************
 * Light_itf.c
 *
 * generated by : embedded software producer/uml statemachine 0.1.5
 **********************************************************************/

#include "Light.h"

#define STATEMACHINE Light_struct

/**********************************************************************
 * handy helpers
 **********************************************************************/
#define light_snd	instance->light_snd
#define pout_io  	instance->pout_io
#define poutData 	instance->poutData

/**********************************************************************
 * Light memory hosting all related information for its own execution
 **********************************************************************/
static int current_index = 0; /* static counter of instances */

 /* array sized with the number of instances */
static STATEMACHINE component_inst_array[NB_INST_LIGHT];

/**********************************************************************
 * Init function invoked for component initialization purpose.
 **********************************************************************/
void Light_init(Rte_Instance inst) {
	STATEMACHINE *instance;
	
	/* we store an instance of the Light memory in "user_section" area */
	inst->user_section = (void*)&component_inst_array[current_index++];
	
	/* Init instance once user_section is allocated */
	instance = (STATEMACHINE*)inst->user_section;
	
	/* Initialization job which can be done in a separated function or inlined. */
	_basic_Light_init(instance);
}

/**********************************************************************
 * Main function Body of state machine "Light"
 **********************************************************************/
void Light(Rte_Instance inst) {
	STATEMACHINE *instance = (STATEMACHINE*)inst->user_section;
	
	/* read input(s) */
	RTE_Read_light_snd(inst, &(light_snd));
	
	/* handle event(s) */
	instance->EVENT = LIGHT_EVENT_T_NO_EVENT;
	if(instance->EVENT == LIGHT_EVENT_T_NO_EVENT) {
		SignalEvent_Tick_type SignalEvent_Tick;
		RTE_Read_InputSignal_Tick(inst, &SignalEvent_Tick);
		if(SignalEvent_Tick.trigger == TRUE) {instance->EVENT = LIGHT_EVENT_T_Tick;}
	}
	
	/* update WALL_CLOCK */
	instance->WALL_CLOCK += inst->period;
	
	/* perform step */
	_basic_Light(instance);
	
	/* Copy data out */
	pout_io = poutData;
	
	/* write output(s) */
	RTE_Write_pout_io(inst, &pout_io);
}

/**********************************************************************
 * Exit function invoked at execution ending
 **********************************************************************/
void Light_end(Rte_Instance inst) {
	STATEMACHINE *instance = (STATEMACHINE*)inst->user_section;
	_basic_Light_end(instance);
}

/* end of file */