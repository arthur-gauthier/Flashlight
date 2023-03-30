/*2023-03-30T15:08:15.035925420Z*/

/**********************************************************************
 * Light.h
 *
 * generated by : embedded software producer/uml statemachine 0.1.5
 **********************************************************************/

#ifndef LIGHT_H
#define LIGHT_H

#if defined( __cplusplus) || (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L))
#include <stdbool.h>
#else
#ifndef bool
#define bool	int
#endif

#ifndef true
#define true	(1==1)
#endif

#ifndef false
#define false	(!true)
#endif
#endif /* stdbool check */

#include "Light_itf.h"

/* BEG_USER_CODE(types) do not remove */

typedef double	WALLCLOCK_T;  /* time type (can be any scalar) */
typedef void *	USERDATA_T;   /* replace this pointer by an anonymous struct if needed. */
#ifndef Boolean_DEFINITION
#define Boolean_DEFINITION
typedef bool	Boolean;
#endif
#ifndef Integer_DEFINITION
#define Integer_DEFINITION
typedef int  	Integer;
#endif
#ifndef Real_DEFINITION
#define Real_DEFINITION
typedef float 	Real;
#endif
#ifndef String_DEFINITION
#define String_DEFINITION
typedef char *	String;
#endif
typedef union {
	Integer	i;
	Real	f;
	String	s;
	void *	v;
} ANYTYPE_T;

/* END_USER_CODE(types) do not remove */

#ifndef TRUE
#define TRUE	true
#endif

#ifndef FALSE
#define FALSE	false
#endif

typedef enum {
	LIGHT_STATE_T_INITIAL_0,
	LIGHT_STATE_T_Off,
	LIGHT_STATE_T_On,
	LIGHT_STATE_T_VOID
} LIGHT_STATE_T;

typedef enum {
	LIGHT_EVENT_T_Tick,
	LIGHT_EVENT_T_NO_EVENT
} LIGHT_EVENT_T;

typedef struct {
	/* user data */
	USERDATA_T 	USERDATA;
	
	/* ports */
	Boolean    	light_snd;
	Boolean    	pout_io;
	
	/* properties */
	Boolean    	poutData;
	
	/* sub behaviors */
	
	/* internal */
	LIGHT_STATE_T	CURRENT_STATE_0;
	WALLCLOCK_T	WALL_CLOCK;
	LIGHT_EVENT_T	EVENT;
	Boolean    	FIRST;
	Boolean    	DONE_ACTIVITY;
} Light_struct;

void _basic_Light_init(Light_struct *instance);
void _basic_Light(Light_struct *instance);
void _basic_Light_end(Light_struct *instance);

/* BEG_USER_CODE(extra-header) do not remove */

/* insert here things you need to add to the header file */

/* END_USER_CODE(extra-header) do not remove */

#endif /*LIGHT_H*/
/* end of file */