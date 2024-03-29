/*2023-03-30T15:07:49.432507902Z*/

/**********************************************************************
 * Timer_usr.c
 *
 * generated by : embedded software producer/uml statemachine 0.1.5
 **********************************************************************/

#ifdef STATEMACHINE

/**********************************************************************
 * handy helpers
 **********************************************************************/
#define light_snd  	instance->light_snd
#define control_snd	instance->control_snd

/* BEG_USER_CODE(header) do not remove */

/* user-modifiable header part */

/* END_USER_CODE(header) do not remove */

/**
 * Function implementing "Entry" of State:
 * Ticking
 * @generated from: _2022x_1ad00ce_1659687979394_415125_3932, _2022x_1ad00ce_1659689071378_428346_4514
 */
static void Ticking_Entry(STATEMACHINE *instance) {
/* BEG_USER_CODE(Entry_2022x_1ad00ce_1659687979394_415125_3932) do not remove */

	_basic_Timer_Send_Tick(&(instance->Ticking_Entry_Send_Tick));

/* END_USER_CODE(Entry_2022x_1ad00ce_1659687979394_415125_3932) do not remove */
}

static void StateMachine_init(STATEMACHINE *instance) {
/* BEG_USER_CODE(init_state_machine) do not remove */

	/* do whatever needed */
	_basic_Timer_Send_Tick_init(&(instance->Ticking_Entry_Send_Tick));

/* END_USER_CODE(init_state_machine) do not remove */
}

static void StateMachine_end(STATEMACHINE *instance) {
/* BEG_USER_CODE(end_state_machine) do not remove */

	/* do whatever needed */
	_basic_Timer_Send_Tick_end(&(instance->Ticking_Entry_Send_Tick));

/* END_USER_CODE(end_state_machine) do not remove */
}

/* BEG_USER_CODE(footer) do not remove */

/* user-modifiable footer part */

/* END_USER_CODE(footer) do not remove */

#endif /* STATEMACHINE */
/* end of file */
