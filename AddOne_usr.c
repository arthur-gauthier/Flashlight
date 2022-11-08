/*2022-11-03T16:29:03.330Z*/

/**********************************************************************
 * AddOne_usr.c
 *
 * generated by : embedded software producer/uml statemachine 0.1.5
 **********************************************************************/

#ifdef STATEMACHINE

/**********************************************************************
 * handy helpers
 **********************************************************************/
#define input_flow1 	instance->input_flow1
#define input_flow2 	instance->input_flow2
#define output_flow1	instance->output_flow1
#define output_flow2	instance->output_flow2
#define inputData1  	instance->inputData1
#define inputData2  	instance->inputData2
#define outputData1 	instance->outputData1
#define outputData2 	instance->outputData2

BEG_USER_CODE(header)

/* user-modifiable header part */

END_USER_CODE(header)

static void running_Entry(STATEMACHINE *instance) {
	BEG_USER_CODE(Entry_19_0_3_16600ad_1654593633995_164496_43115)
	
	outputData1 = inputData1+1;
outputData2 = inputData2+2;
	
	END_USER_CODE(Entry_19_0_3_16600ad_1654593633995_164496_43115)
}

static void StateMachine_init(STATEMACHINE *instance) {
	inputData1  = 0;
	inputData2  = 0;
	outputData1 = 0;
	outputData2 = 0;
	
	BEG_USER_CODE(init_state_machine)
	
	/* do whatever needed */
	
	END_USER_CODE(init_state_machine)
}

static void StateMachine_end(STATEMACHINE *instance) {
	BEG_USER_CODE(end_state_machine)
	
	/* do whatever needed */
	
	END_USER_CODE(end_state_machine)
}

BEG_USER_CODE(footer)

/* user-modifiable footer part */

END_USER_CODE(footer)

#endif /* STATEMACHINE */
/* end of file */