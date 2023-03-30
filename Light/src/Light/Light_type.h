/*2023-03-30T15:08:14.959444336Z*/

/**********************************************************************
 * Light_type.h
 * Header File containing the definition of types for component: Light
 *
 * generated by : embedded software producer / runtime code gen 1.0.0
 **********************************************************************/

#ifndef Light_TYPE_H
#define Light_TYPE_H

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

/**********************************************************************
 * Data Type Boolean definition
 **********************************************************************/
#ifndef Boolean_DEFINITION
#define Boolean_DEFINITION
typedef bool Boolean;
#endif /* Boolean */
/**********************************************************************
 * Data Type trigger_type definition
 **********************************************************************/
#ifndef trigger_type_DEFINITION
#define trigger_type_DEFINITION
typedef bool trigger_type;
#endif /* trigger_type */
/**********************************************************************
 * Data Type SignalEvent_On_type definition
 **********************************************************************/
#ifndef SignalEvent_On_type_DEFINITION
#define SignalEvent_On_type_DEFINITION
typedef struct{
	trigger_type trigger;
} SignalEvent_On_type;
#endif /* SignalEvent_On_type */
/**********************************************************************
 * Data Type SignalEvent_Off_type definition
 **********************************************************************/
#ifndef SignalEvent_Off_type_DEFINITION
#define SignalEvent_Off_type_DEFINITION
typedef struct{
	trigger_type trigger;
} SignalEvent_Off_type;
#endif /* SignalEvent_Off_type */
/**********************************************************************
 * Data Type SignalEvent_Tick_type definition
 **********************************************************************/
#ifndef SignalEvent_Tick_type_DEFINITION
#define SignalEvent_Tick_type_DEFINITION
typedef struct{
	trigger_type trigger;
} SignalEvent_Tick_type;
#endif /* SignalEvent_Tick_type */

#endif /* Light_TYPE_H */
