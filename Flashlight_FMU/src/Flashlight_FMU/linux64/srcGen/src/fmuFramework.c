/////////////////////////////////////////////////////////////////////////////
//
//    fmuFramework.c
//    fmuFramework for FMI 2.0
//
//    author: Dassault Syst?mes
//      creation: 2016-12
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>


#include "fmuFramework.h"
#include "fmuSpecific.h"

#ifdef _WIN32
	#include <windows.h>
	#include <process.h>
#else  // _WIN32 //
	#include <pthread.h>
	#include <string.h>
	#include <sys/stat.h>
#endif // _WIN32 //

#define QUOTE_(X) #X
#define QUOTE(X) QUOTE_(X)


#ifdef _WIN32
	HANDLE multiInstancesSem = NULL;
#else  // _WIN32 //
	pthread_mutex_t multiInstancesSem;
#endif // _WIN32 //


/////////////////////////////////////////////////////////////////////////////
// Private helpers used below to validate function arguments
/////////////////////////////////////////////////////////////////////////////
static fmi2Boolean invalidNumber(FMUInstance* fmuInstance, fmi2String f, fmi2String arg, size_t n, int nExpected) {
	if (n != (size_t)nExpected) {
		fmuInstance->status = fmuError;
		fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Error, "error", "%s: Invalid argument %s = %d. Expected %d.", f, arg, n, nExpected);
		return fmi2True;
	}
	return fmi2False;
}

static fmi2Boolean invalidState(FMUInstance* fmuInstance, fmi2String f, int statusExpected) {
	if (!fmuInstance) return fmi2True;
	if (!(fmuInstance->status & statusExpected)) {
		fmuInstance->status = fmuError;
		fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Error, "error", "%s: Illegal call sequence.", f);
		return fmi2True;
	}
	return fmi2False;
}

static fmi2Boolean nullPointer(FMUInstance* fmuInstance, fmi2String f, fmi2String arg, const void* p) {
	if (!p) {
		fmuInstance->status = fmuError;
		fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Error, "error", "%s: Invalid argument %s = NULL.", f, arg);
		return fmi2True;
	}
	return fmi2False;
}

fmi2Char formatedString[513];
static fmi2String formatStringForLog(fmi2String s) {
	int i, iOffset=0;
	for(i=0; s[i]!='#'; i++) {
		if(s[i] == '\0') return s;
	}
	for(i=0; s[i]!='\0'; i++) {
		formatedString[i+iOffset] = s[i];
		if(s[i] == '#') formatedString[i+ ++iOffset] = '#';
	}
	formatedString[i+iOffset] = '\0';
	return formatedString;
}

/////////////////////////////////////////////////////////////////////////////
// Private useful functions
/////////////////////////////////////////////////////////////////////////////
void fmu_createCriticalSection() {
#ifdef _WIN32
	if( !multiInstancesSem ) {
		char semName[256];
		sprintf(semName, "%s_sem", QUOTE(MODEL_IDENTIFIER));
		multiInstancesSem = CreateSemaphore(NULL, 1, 1, semName);
	}
#else // _WIN32 //
	pthread_mutex_init(&multiInstancesSem, NULL);
#endif // _WIN32 //
}

void fmu_enterCriticalSection(FMUInstance* fmuInstance) {
	(void) fmuInstance;
#ifdef _WIN32
	WaitForSingleObject(multiInstancesSem, INFINITE);
#else // _WIN32 //
	pthread_mutex_lock(&multiInstancesSem);
#endif // _WIN32 //
}

void fmu_exitCriticalSection(FMUInstance* fmuInstance) {
	(void) fmuInstance;
#ifdef _WIN32
	ReleaseSemaphore(multiInstancesSem, 1, NULL);
#else // _WIN32 //
	pthread_mutex_unlock(&multiInstancesSem);
#endif // _WIN32 //
}

/////////////////////////////////////////////////////////////////////////////
// With FMI 1.0 fmuLocation should be something like:
//   ...\temp\...\binaries\winXX\fmuDriver.dll
//               \resources\fmuDriver.cfg
/////////////////////////////////////////////////////////////////////////////
//RE//TODO//TOUPDATE
#define MAX_PATH_LEN 1024
char acExtractedPath[MAX_PATH_LEN] = {'\0'};
#ifdef _WIN32
const char* defaultSubPaths[] = {"\\resources", "\\..\\..\\resources", "", NULL};
#else  // _WIN32 //
const char* defaultSubPaths[] = {"/resources", "/../../resources", "", NULL};
#endif // _WIN32 //
char* getFilePath(FMUInstance* fmuInstance, const char* acFilename) {
	fmi2String fmuLocation = fmuInstance->fmuLocation;
#ifdef _WIN32
	size_t iSeparator, fmuLocationLen;
	if(!fmuLocation || fmuLocation[0]=='\0') {
		fmuLocation = (fmi2String) getenv("FMU_DIR");
		if(!fmuLocation || fmuLocation[0]=='\0') GetModuleFileName(NULL, acExtractedPath, MAX_PATH_LEN);
		else                                     strncpy(acExtractedPath, fmuLocation, MAX_PATH_LEN - 1);
	} else {
		if( !strncmp("file:/", fmuLocation, 6) ) {
			// RFC3986 case
			if( !strncmp("file:///", fmuLocation, 8) ) strncpy(acExtractedPath, fmuLocation+8, MAX_PATH_LEN - 1);
			else                                       strncpy(acExtractedPath, fmuLocation+6, MAX_PATH_LEN - 1);
			for(iSeparator=0; acExtractedPath[iSeparator]!='\0'; iSeparator++) if(acExtractedPath[iSeparator] == '/') acExtractedPath[iSeparator] = '\\';
			if(iSeparator>=1 && acExtractedPath[1]=='|') acExtractedPath[1] = ':';
		} else {
			strncpy(acExtractedPath, fmuLocation, MAX_PATH_LEN - 1);
		}
	}
	fmuLocationLen = strlen(acExtractedPath);
	if(fmuLocationLen>0 && acExtractedPath[fmuLocationLen-1]=='\\')     iSeparator = fmuLocationLen - 1;
	else if(fmuLocationLen>4 && acExtractedPath[fmuLocationLen-4]=='.') for(iSeparator = fmuLocationLen-1; (iSeparator>=0 && acExtractedPath[iSeparator]!='\\'); iSeparator--);
	else                                                                iSeparator = fmuLocationLen;
	if(iSeparator > 0) {
		int iPath = 0;
		FILE* pFile = (FILE*) NULL;
		while( defaultSubPaths[iPath] ) {
			sprintf(acExtractedPath+iSeparator, "%s\\%s", defaultSubPaths[iPath], acFilename);
			pFile = fopen(acExtractedPath, "r");
			if ( pFile ) {
				fclose(pFile);
				break;
			}
			iPath++;
		}
		if( !defaultSubPaths[iPath] ) acExtractedPath[0] = '\0';
	}
	if(acExtractedPath[0] == '\0') {
		FILE* pFile = (FILE*) NULL;
		sprintf(acExtractedPath, "C:\\tmp\\%s", acFilename);
		pFile = fopen(acExtractedPath, "r");
		if ( pFile ) {
			fclose(pFile);
		} else {
			sprintf(acExtractedPath, "%s", acFilename);
		}
	}
#else  // _WIN32 //
	size_t iSeparator, fmuLocationLen;
	if(!fmuLocation || fmuLocation[0]=='\0') {
		fmuLocation = (fmi2String) getenv("FMUDRIVER_DIR");
		if(!fmuLocation || fmuLocation[0]=='\0') {
			strncpy(acExtractedPath, fmuLocation, MAX_PATH_LEN - 1);
		}
	} else {
		if( !strncmp("file://", fmuLocation, 7) ) {
			// RFC3986 case
			strncpy(acExtractedPath, fmuLocation+7, MAX_PATH_LEN - 1);
		} else {
			strncpy(acExtractedPath, fmuLocation, MAX_PATH_LEN - 1);
		}
	}
	fmuLocationLen = strlen(acExtractedPath);
	if(fmuLocationLen>0 && acExtractedPath[fmuLocationLen-1]=='/')      iSeparator = fmuLocationLen - 1;
	else if(fmuLocationLen>4 && acExtractedPath[fmuLocationLen-3]=='.') for(iSeparator = fmuLocationLen-1; (iSeparator>=0 && acExtractedPath[iSeparator]!='/'); iSeparator--);
	else                                                                iSeparator = fmuLocationLen;
	if(iSeparator > 0) {
		int iPath = 0;
		struct stat pFileStat;
		FILE* pFile = (FILE*) NULL;
		while( defaultSubPaths[iPath] ) {
			sprintf(acExtractedPath+iSeparator, "%s/%s", defaultSubPaths[iPath], acFilename);
			stat(acExtractedPath, &pFileStat);
			if (S_ISREG(pFileStat.st_mode) && (pFile=fopen(acExtractedPath, "r"))) {
				fclose(pFile);
				break;
			}
			iPath++;
		}
		if( !defaultSubPaths[iPath] ) acExtractedPath[0] = '\0';
	}
	if(acExtractedPath[0] == '\0') {
		struct stat pFileStat;
		FILE* pFile = (FILE*) NULL;
		sprintf(acExtractedPath, "/tmp/%s", acFilename);
		stat(acExtractedPath, &pFileStat);
		if (S_ISREG(pFileStat.st_mode) && (pFile=fopen(acExtractedPath, "r"))) {
			fclose(pFile);
		} else {
			sprintf(acExtractedPath, "%s", acFilename);
		}
	}
#endif // _WIN32 //
	return acExtractedPath;
}

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus //

/////////////////////////////////////////////////////////////////////////////
// FMI: inquire version numbers of header files and setting logging status
/////////////////////////////////////////////////////////////////////////////
const char* fmi2GetTypesPlatform() {
	return fmi2TypesPlatform;
}

const char* fmi2GetVersion() {
	return fmi2Version;
}

fmi2Status fmi2SetDebugLogging(fmi2Component c, fmi2Boolean loggingOn, size_t nCategories, const fmi2String categories[]) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	(void) nCategories;
	(void) categories;
	if (invalidState(fmuInstance, "fmi2SetDebugLogging", not_fmuError)) return fmi2Error;
	if (fmuInstance->debugLoggingOn || loggingOn) {
		fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2SetDebugLogging: loggingOn=%d", loggingOn);
	}
	fmuInstance->debugLoggingOn = loggingOn;
	return fmi2OK;
}

/////////////////////////////////////////////////////////////////////////////
// FMI: Creation and destruction of FMU instances and setting debug status
/////////////////////////////////////////////////////////////////////////////
static int nbFMUsInstantiated = 0;
fmi2Component fmi2Instantiate(fmi2String instanceName, fmi2Type fmuType, fmi2String fmuGUID, fmi2String fmuResourceLocation, const fmi2CallbackFunctions* functions, fmi2Boolean visible, fmi2Boolean loggingOn) {
	FMUInstance* fmuInstance;
	size_t       instanceNameLen = strlen(instanceName) + 1;
	size_t       fmuLocationLen =  0;
	(void) fmuType;
	(void) visible;

	fmu_createCriticalSection();
	fmu_enterCriticalSection(NULL);

	if (!functions->logger) {
		fmu_exitCriticalSection(NULL);
		return NULL;
	}
	if (!functions->allocateMemory || !functions->freeMemory) {
		functions->logger(functions->componentEnvironment, instanceName, fmi2Fatal, "error", "fmi2Instantiate: Missing callback function.");
		fmu_exitCriticalSection(NULL);
		return NULL;
	}
	if (!instanceName || strlen(instanceName)==0) {
		functions->logger(functions->componentEnvironment, instanceName, fmi2Fatal, "error", "fmi2Instantiate: Missing instance name.");
		fmu_exitCriticalSection(NULL);
		return NULL;
	}
	if (strcmp(fmuGUID, QUOTE(MODEL_GUID))) {
		functions->logger(functions->componentEnvironment, instanceName, fmi2Fatal, "error", "fmi2Instantiate: Wrong GUID %s. Expected %s.", fmuGUID, QUOTE(MODEL_GUID));
		fmu_exitCriticalSection(NULL);
		return NULL;
	}
	fmuInstance = (FMUInstance *)functions->allocateMemory(1, sizeof(FMUInstance));
	if ( !fmuInstance ) {
		functions->logger(functions->componentEnvironment, instanceName, fmi2Fatal, "error", "fmi2Instantiate: Memory allocation failed.");
		fmu_exitCriticalSection(NULL);
		return NULL;
	}
	fmuInstance->instanceName = (fmi2String)functions->allocateMemory(instanceNameLen, sizeof(fmi2Char));
	if ( !fmuInstance->instanceName ) {
		functions->logger(functions->componentEnvironment, instanceName, fmi2Fatal, "error", "fmi2Instantiate: Memory allocation failed.");
		fmu_exitCriticalSection(NULL);
		return NULL;
	}
	if(fmuResourceLocation && (fmuLocationLen = strlen(fmuResourceLocation)+1)>1) {
		fmuInstance->fmuLocation = (fmi2String)functions->allocateMemory(fmuLocationLen, sizeof(fmi2Char));
		if ( !fmuInstance->fmuLocation ) {
			functions->logger(functions->componentEnvironment, instanceName, fmi2Fatal, "error", "fmi2Instantiate: Memory allocation failed.");
			fmu_exitCriticalSection(NULL);
			return NULL;
		}
	}
	if ( loggingOn ) functions->logger(functions->componentEnvironment, instanceName, fmi2OK, "log", "fmi2Instantiate: GUID=%s", fmuGUID);

#if defined(MULTI_INSTANTIATION_NOT_ALLOWED) && MULTI_INSTANTIATION_NOT_ALLOWED
	if(nbFMUsInstantiated >= 1) {
		functions->logger(functions->componentEnvironment, instanceName, fmi2Fatal, "error", "fmi2Instantiate: multi-instantiation is not supported.");
		fmu_exitCriticalSection(NULL);
		return NULL;
	}
#endif // defined(MULTI_INSTANTIATION_NOT_ALLOWED) && MULTI_INSTANTIATION_NOT_ALLOWED //
	++nbFMUsInstantiated;

	strncpy((char*)fmuInstance->instanceName, (char*)instanceName, instanceNameLen*sizeof(fmi2Char));
	if(fmuLocationLen > 1) strncpy((char*)fmuInstance->fmuLocation, (char*)fmuResourceLocation, fmuLocationLen*sizeof(fmi2Char));
	fmuInstance->logger               = functions->logger;
	fmuInstance->allocateMemory       = functions->allocateMemory;
	fmuInstance->freeMemory           = functions->freeMemory;
	fmuInstance->stepFinished         = functions->stepFinished;
	fmuInstance->componentEnvironment = functions->componentEnvironment;
	fmuInstance->debugLoggingOn       = loggingOn;
	fmuInstance->status               = fmuInstantiated;
	fmuInstance->time                 = 0.0;
	fmuInstance->privateCtx           = NULL;

	fmu_exitCriticalSection(NULL);

	fmuInstanciate(fmuInstance);

	return fmuInstance;
}

void fmi2FreeInstance(fmi2Component c) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	if( !fmuInstance ) return;

	if( fmuInstance->debugLoggingOn ) {
		fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2FreeInstance");
	}
	fmuInstance->freeMemory((void*)fmuInstance->instanceName);
	if( fmuInstance->fmuLocation ) fmuInstance->freeMemory((void*)fmuInstance->fmuLocation);
	fmuInstance->freeMemory(fmuInstance);
	nbFMUsInstantiated--;
}

//////////////////////////////////////////////////////////////////////////////
// FMI: Enter and exit initialization mode, terminate and reset
//////////////////////////////////////////////////////////////////////////////
fmi2Status fmi2SetupExperiment(fmi2Component c, fmi2Boolean toleranceDefined, fmi2Real tolerance, fmi2Real startTime, fmi2Boolean stopTimeDefined, fmi2Real stopTime) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	(void) toleranceDefined;
	(void) tolerance;
	(void) stopTimeDefined;
	(void) stopTime;

	if (invalidState(fmuInstance, "fmi2SetupExperiment", fmuInstantiated)) return fmi2Error;
	if (fmuInstance->debugLoggingOn) fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2SetupExperiment");

	fmuInstance->time = startTime;

	return fmi2OK;
}

fmi2Status fmi2EnterInitializationMode(fmi2Component c) {
	FMUInstance* fmuInstance = (FMUInstance *)c;

	if (invalidState(fmuInstance, "fmi2EnterInitializationMode", fmuInstantiated)) return fmi2Error;
	if (fmuInstance->debugLoggingOn) fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2EnterInitializationMode");

	fmu_enterCriticalSection(fmuInstance);
//TODO: do initialization
	fmuInit(fmuInstance);
	fmu_exitCriticalSection(fmuInstance);
	fmuInstance->status = fmuInitializing;

	return fmi2OK;
}

fmi2Status fmi2ExitInitializationMode(fmi2Component c) {
	FMUInstance* fmuInstance = (FMUInstance *)c;

	if (invalidState(fmuInstance, "fmi2ExitInitializationMode", fmuInitializing)) return fmi2Error;
	if (fmuInstance->debugLoggingOn) fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2ExitInitializationMode");

	fmu_enterCriticalSection(fmuInstance);
//TODO: finalizing initialization
	fmuFinalizeInit(fmuInstance);
	fmu_exitCriticalSection(fmuInstance);
	fmuInstance->status = fmuInitialized;

	return fmi2OK;
}

fmi2Status fmi2Terminate(fmi2Component c) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	if (invalidState(fmuInstance, "fmi2Terminate", fmuInitialized)) return fmi2Error;
	if (fmuInstance->debugLoggingOn) {
		fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2Terminate");
	}

	fmu_enterCriticalSection(fmuInstance);
//TODO: do terminate
	fmuUninit(fmuInstance);
	fmuInstance->status = fmuTerminated;
	fmu_exitCriticalSection(fmuInstance);

	return fmi2OK;
}

fmi2Status fmi2Reset(fmi2Component c) {
	FMUInstance* fmuInstance = (FMUInstance *)c;

	if (invalidState(fmuInstance, "fmi2Reset", fmuInitialized)) return fmi2Error;
	if (fmuInstance->debugLoggingOn) {
		fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2ResetSlave");
	}

	fmu_enterCriticalSection(fmuInstance);
//TODO: do reset
	fmuUninit(fmuInstance);
	fmuInit(fmuInstance);
	fmuInstance->status = fmuInstantiated;
	fmu_exitCriticalSection(fmuInstance);

	return fmi2OK;
}

/////////////////////////////////////////////////////////////////////////////
// FMI: Getting variable values
/////////////////////////////////////////////////////////////////////////////
fmi2Status fmi2GetReal(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Real value[]) {
	size_t i;
	FMUInstance* fmuInstance = (FMUInstance *)c;

	if ( invalidState(fmuInstance, "fmi2GetReal", not_fmuError) )           return fmi2Error;
	if (nvr>0 && nullPointer(fmuInstance, "fmi2GetReal", "vr[]", vr))       return fmi2Error;
	if (nvr>0 && nullPointer(fmuInstance, "fmi2GetReal", "value[]", value)) return fmi2Error;
	for (i=0; i<nvr; i++) {
//TODO: get variable
		value[i] = getFmuReal(fmuInstance, vr[i]);
		if (fmuInstance->debugLoggingOn) {
			fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2GetReal: #r%u# = %.16g", vr[i], value[i]);
		}
	}

	return fmi2OK;
}

fmi2Status fmi2GetInteger(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Integer value[]) {
	size_t i;
	FMUInstance* fmuInstance = (FMUInstance *)c;

	if ( invalidState(fmuInstance, "fmi2GetInteger", not_fmuError) )           return fmi2Error;
	if (nvr>0 && nullPointer(fmuInstance, "fmi2GetInteger", "vr[]", vr))       return fmi2Error;
	if (nvr>0 && nullPointer(fmuInstance, "fmi2GetInteger", "value[]", value)) return fmi2Error;
	for (i=0; i<nvr; i++) {
//TODO: get variable
		value[i] = getFmuInteger(fmuInstance, vr[i]);
		if (fmuInstance->debugLoggingOn) {
			fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2GetInteger: #i%u# = %d", vr[i], value[i]);
		}
	}

	return fmi2OK;
}

fmi2Status fmi2GetBoolean(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Boolean value[]) {
	size_t i;
	FMUInstance* fmuInstance = (FMUInstance *)c;

	if ( invalidState(fmuInstance, "fmi2GetBoolean", not_fmuError) )           return fmi2Error;
	if (nvr>0 && nullPointer(fmuInstance, "fmi2GetBoolean", "vr[]", vr))       return fmi2Error;
	if (nvr>0 && nullPointer(fmuInstance, "fmi2GetBoolean", "value[]", value)) return fmi2Error;
	for (i=0; i<nvr; i++) {
//TODO: get variable
		value[i] = getFmuBoolean(fmuInstance, vr[i]);
		if (fmuInstance->debugLoggingOn) {
			fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2GetBoolean: #b%u# = %s", vr[i], value[i]? "true" : "false");
		}
	}

	return fmi2OK;
}

fmi2Status fmi2GetString(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2String  value[]) {
	size_t i;
	FMUInstance* fmuInstance = (FMUInstance *)c;

	if ( invalidState(fmuInstance, "fmi2GetString", not_fmuError) )           return fmi2Error;
	if (nvr>0 && nullPointer(fmuInstance, "fmi2GetString", "vr[]", vr))       return fmi2Error;
	if (nvr>0 && nullPointer(fmuInstance, "fmi2GetString", "value[]", value)) return fmi2Error;
	for (i=0; i<nvr; i++) {
//TODO: get variable
		value[i] = getFmuString(fmuInstance, vr[i]);
		if (fmuInstance->debugLoggingOn) {
			fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2GetString: #s%u# = '%s'", vr[i], formatStringForLog(value[i]));
		}
	}
	return fmi2OK;
}

/////////////////////////////////////////////////////////////////////////////
// FMI: Setting variable values
/////////////////////////////////////////////////////////////////////////////
fmi2Status fmi2SetReal(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Real value[]) {
	size_t i;
	FMUInstance* fmuInstance = (FMUInstance *)c;

	if (invalidState(fmuInstance, "fmi2SetReal", fmuInstantiated|fmuInitialized|fmuInitializing)) return fmi2Error;
	if (nvr>0 && nullPointer(fmuInstance, "fmi2SetReal", "vr[]", vr))             return fmi2Error;
	if (nvr>0 && nullPointer(fmuInstance, "fmi2SetReal", "value[]", value))       return fmi2Error;
	if (fmuInstance->debugLoggingOn) {
		fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2SetReal: nvr = %d", nvr);
	}
	for (i=0; i<nvr; i++) {
		if (fmuInstance->debugLoggingOn) {
			fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2SetReal: #r%d# = %.16g", vr[i], value[i]);
		}
//TODO: set variable
		setFmuReal(fmuInstance, vr[i], value[i]);
	}

	return fmi2OK;
}

fmi2Status fmi2SetInteger(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Integer value[]) {
	size_t i;
	FMUInstance* fmuInstance = (FMUInstance *)c;

	if (invalidState(fmuInstance, "fmi2SetInteger", fmuInstantiated|fmuInitialized|fmuInitializing)) return fmi2Error;
	if (nvr>0 && nullPointer(fmuInstance, "fmi2SetInteger", "vr[]", vr))             return fmi2Error;
	if (nvr>0 && nullPointer(fmuInstance, "fmi2SetInteger", "value[]", value))       return fmi2Error;
	if (fmuInstance->debugLoggingOn) {
		fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2SetInteger: nvr = %d",  nvr);
	}
	for (i=0; i<nvr; i++) {
		if (fmuInstance->debugLoggingOn) {
			fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2SetInteger: #i%d# = %d", vr[i], value[i]);
		}
//TODO: set variable
		setFmuInteger(fmuInstance, vr[i], value[i]);
	}

	return fmi2OK;
}

fmi2Status fmi2SetBoolean(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Boolean value[]) {
	size_t i;
	FMUInstance* fmuInstance = (FMUInstance *)c;

	if (invalidState(fmuInstance, "fmi2SetBoolean", fmuInstantiated|fmuInitialized|fmuInitializing)) return fmi2Error;
	if (nvr>0 && nullPointer(fmuInstance, "fmi2SetBoolean", "vr[]", vr))             return fmi2Error;
	if (nvr>0 && nullPointer(fmuInstance, "fmi2SetBoolean", "value[]", value))       return fmi2Error;
	if (fmuInstance->debugLoggingOn) {
		fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2SetBoolean: nvr = %d",  nvr);
	}
	for (i=0; i<nvr; i++) {
		if (fmuInstance->debugLoggingOn) {
			fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2SetBoolean: #b%d# = %s", vr[i], value[i] ? "true" : "false");
		}
//TODO: set variable
		setFmuBoolean(fmuInstance, vr[i], value[i]);
	}

	return fmi2OK;
}

fmi2Status fmi2SetString(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2String value[]) {
	size_t i;
	FMUInstance* fmuInstance = (FMUInstance *)c;

	if (invalidState(fmuInstance, "fmi2SetString", fmuInstantiated|fmuInitialized|fmuInitializing)) return fmi2Error;
	if (nvr>0 && nullPointer(fmuInstance, "fmi2SetString", "vr[]", vr))             return fmi2Error;
	if (nvr>0 && nullPointer(fmuInstance, "fmi2SetString", "value[]", value))       return fmi2Error;
	if (fmuInstance->debugLoggingOn) {
		fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2SetString: nvr = %d",  nvr);
	}
	for (i=0; i<nvr; i++) {
		if (nullPointer(fmuInstance, "fmi2SetString", "value[i]", value[i])) return fmi2Error;
		if (fmuInstance->debugLoggingOn) {
			fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2SetString: #s%u# = '%s'", vr[i], formatStringForLog(value[i]));
		}
//TODO: set variable
		setFmuString(fmuInstance, vr[i], value[i]);
	}

	return fmi2OK;
}

//////////////////////////////////////////////////////////////////////////////
// FMI: Getting and setting the internal FMU state
//////////////////////////////////////////////////////////////////////////////
fmi2Status fmi2GetFMUstate(fmi2Component c, fmi2FMUstate* s) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	if (invalidState(fmuInstance, "fmi2GetFMUstate", not_fmuError)) return fmi2Error;
	fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Error, "error", "fmi2GetFMUstate: Function not supported.");
	return fmi2Discard;
}

fmi2Status fmi2SetFMUstate(fmi2Component c, fmi2FMUstate s) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	if (invalidState(fmuInstance, "fmi2SetFMUstate", not_fmuError)) return fmi2Error;
	fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Error, "error", "fmi2SetFMUstate: Function not supported.");
	return fmi2Discard;
}

fmi2Status fmi2FreeFMUstate(fmi2Component c, fmi2FMUstate* s) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	if (invalidState(fmuInstance, "fmi2FreeFMUstate", not_fmuError)) return fmi2Error;
	fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Error, "error", "fmi2FreeFMUstate: Function not supported.");
	return fmi2Discard;
}

fmi2Status fmi2SerializedFMUstateSize(fmi2Component c, fmi2FMUstate s, size_t* ns) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	if (invalidState(fmuInstance, "fmi2SerializedFMUstateSize", not_fmuError)) return fmi2Error;
	fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Error, "error", "fmi2SerializedFMUstateSize: Function not supported.");
	return fmi2Discard;
}

fmi2Status fmi2SerializeFMUstate(fmi2Component c, fmi2FMUstate s, fmi2Byte b[], size_t nb) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	if (invalidState(fmuInstance, "fmi2SerializeFMUstate", not_fmuError)) return fmi2Error;
	fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Error, "error", "fmi2SerializeFMUstate: Function not supported.");
	return fmi2Discard;
}

fmi2Status fmi2DeSerializeFMUstate(fmi2Component c, const fmi2Byte b[], size_t nb, fmi2FMUstate* s) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	if (invalidState(fmuInstance, "fmi2DeSerializeFMUstate", not_fmuError)) return fmi2Error;
	fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Error, "error", "fmi2DeSerializeFMUstate: Function not supported.");
	return fmi2Discard;
}

//////////////////////////////////////////////////////////////////////////////
// FMI MoEx: Enter and exit the different modes
//////////////////////////////////////////////////////////////////////////////
fmi2Status fmi2EnterEventMode(fmi2Component c) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	if (invalidState(fmuInstance, "fmi2EnterEventMode", not_fmuError)) return fmi2Error;
	fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Discard, "warning", "fmi2EnterEventMode: Function not supported.");
	return fmi2Discard;
}

fmi2Status fmi2NewDiscreteStates(fmi2Component c, fmi2EventInfo* fmiEventInfo) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	(void) fmiEventInfo;
	if (invalidState(fmuInstance, "fmi2NewDiscreteStates", not_fmuError)) return fmi2Error;
	fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Discard, "warning", "fmi2NewDiscreteStates: Function not supported.");
	return fmi2Discard;
}

fmi2Status fmi2EnterContinuousTimeMode(fmi2Component c) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	if (invalidState(fmuInstance, "fmi2EnterContinuousTimeMode", not_fmuError)) return fmi2Error;
	fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Discard, "warning", "fmi2EnterContinuousTimeMode: Function not supported.");
	return fmi2Discard;
}

fmi2Status fmi2CompletedIntegratorStep(fmi2Component c, fmi2Boolean noSetFMUStatePriorToCurrentPoint, fmi2Boolean* enterEventMode, fmi2Boolean* terminateSimulation) {
	FMUInstance* fmuInstance = (FMUInstance *)c;

	if (invalidState(fmuInstance, "fmi2CompletedIntegratorStep", fmuInitialized)) return fmi2Error;
//RE//	if (nullPointer(fmuInstance, "fmi2CompletedIntegratorStep", "callEventUpdate", callEventUpdate)) return fmi2Error;
	if (fmuInstance->debugLoggingOn) fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2CompletedIntegratorStep");
//RE//	*callEventUpdate = fmi2False;

	return fmi2OK;
}

//////////////////////////////////////////////////////////////////////////////
// FMI MoEx: Providing independent variables and re-initialization of caching
//////////////////////////////////////////////////////////////////////////////
fmi2Status fmi2SetTime(fmi2Component c, fmi2Real time) {
	FMUInstance* fmuInstance = (FMUInstance *)c;

	if (invalidState(fmuInstance, "fmi2SetTime", fmuInstantiated|fmuInitialized)) return fmi2Error;
	if (fmuInstance->debugLoggingOn) fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2SetTime: time=%.16g", time);
	fmuInstance->time = time;

	return fmi2OK;
}

fmi2Status fmi2SetContinuousStates(fmi2Component c, const fmi2Real x[], size_t nx) {
	FMUInstance* fmuInstance = (FMUInstance *)c;

	if (invalidState (fmuInstance, "fmi2SetContinuousStates", fmuInitialized)) return fmi2Error;
	if (invalidNumber(fmuInstance, "fmi2SetContinuousStates", "nx", nx, 0))    return fmi2Error;
	if (nullPointer  (fmuInstance, "fmi2SetContinuousStates", "x[]", x))       return fmi2Error;

	return fmi2OK;
}

//////////////////////////////////////////////////////////////////////////////
// FMI MoEx: Evaluation of the model equations
//////////////////////////////////////////////////////////////////////////////
fmi2Status fmi2GetDerivatives(fmi2Component c, fmi2Real derivatives[], size_t nx) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	if (invalidState(fmuInstance, "fmi2GetDerivatives", not_fmuError))                return fmi2Error;
	if (invalidNumber(fmuInstance, "fmi2GetDerivatives", "nx", nx, 0))                return fmi2Error;
	if (nullPointer(fmuInstance, "fmi2GetDerivatives", "derivatives[]", derivatives)) return fmi2Error;
	return fmi2OK;
}

fmi2Status fmi2GetEventIndicators(fmi2Component c, fmi2Real eventIndicators[], size_t ni) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	if (invalidState(fmuInstance, "fmi2GetEventIndicators", not_fmuError)) return fmi2Error;
	if (invalidNumber(fmuInstance, "fmi2GetEventIndicators", "ni", ni, 0)) return fmi2Error;
	return fmi2OK;
}

fmi2Status fmi2GetContinuousStates(fmi2Component c, fmi2Real states[], size_t nx) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	if (invalidState(fmuInstance, "fmi2GetContinuousStates", not_fmuError))      return fmi2Error;
	if (invalidNumber(fmuInstance, "fmi2GetContinuousStates", "nx", nx, 0))      return fmi2Error;
	if (nullPointer(fmuInstance, "fmi2GetContinuousStates", "states[]", states)) return fmi2Error;
	return fmi2OK;
}

fmi2Status fmi2GetNominalsOfContinuousStates(fmi2Component c, fmi2Real x_nominal[], size_t nx) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	if (invalidState(fmuInstance, "fmi2GetNominalsOfContinuousStates", not_fmuError))            return fmi2Error;
	if (invalidNumber(fmuInstance, "fmi2GetNominalsOfContinuousStates", "nx", nx, 0))            return fmi2Error;
	if (nullPointer(fmuInstance, "fmi2GetNominalsOfContinuousStates", "x_nominal[]", x_nominal)) return fmi2Error;
	x_nominal[0] = 1;
	if (fmuInstance->debugLoggingOn) fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2GetNominalsOfContinuousStates: x_nominal[0..%d] = 1.0", nx-1);
	return fmi2OK;
}

//////////////////////////////////////////////////////////////////////////////
// FMI CoSim: Simulating the slave
//////////////////////////////////////////////////////////////////////////////
fmi2Status fmi2DoStep(fmi2Component c, fmi2Real currentCommunicationPoint, fmi2Real communicationStepSize, fmi2Boolean newStep) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	(void) newStep;
	if (invalidState(fmuInstance, "fmi2DoStep", fmuInitialized)) return fmi2Error;

	fmu_enterCriticalSection(fmuInstance);
//TODO: do step
	fmuStep(fmuInstance, currentCommunicationPoint, communicationStepSize, newStep);
	fmu_exitCriticalSection(fmuInstance);

	return fmi2OK;
}

fmi2Status fmi2CancelStep(fmi2Component c) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	if (fmuInstance->debugLoggingOn) {
		fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2OK, "log", "fmi2CancelStep");
	}

	fmu_enterCriticalSection(fmuInstance);
	fmu_exitCriticalSection(fmuInstance);

	return fmi2OK;
}

/////////////////////////////////////////////////////////////////////////////
// FMI: not supported functions
/////////////////////////////////////////////////////////////////////////////
fmi2Status fmi2GetDirectionalDerivative(fmi2Component c, const fmi2ValueReference vUnknown_ref[], size_t nUnknown, const fmi2ValueReference vKnown_ref[], size_t nKnown, const fmi2Real dvKnown[], fmi2Real dvUnknown[]) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	(void) vUnknown_ref;
	(void) nUnknown;
	(void) vKnown_ref;
	(void) nKnown;
	(void) dvKnown;
	(void) dvUnknown;
	if (invalidState(fmuInstance, "fmi2GetDirectionalDerivative", not_fmuError)) return fmi2Error;
	fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Error, "error", "fmi2GetDirectionalDerivative: Function not supported.");
	return fmi2Discard;
}

fmi2Status fmi2GetStatus(fmi2Component c, const fmi2StatusKind s, fmi2Status* value) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	(void) s;
	(void) value;
	if (invalidState(fmuInstance, "fmi2GetStatus", not_fmuError)) return fmi2Error;
	fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Discard, "warning", "fmi2GetStatus: Function not supported.");
	return fmi2Discard;
}

fmi2Status fmi2GetRealStatus(fmi2Component c, const fmi2StatusKind s, fmi2Real* value) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	(void) s;
	(void) value;
	if (invalidState(fmuInstance, "fmi2GetRealStatus", not_fmuError)) return fmi2Error;
	fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Discard, "warning", "fmi2GetRealStatus: Function not supported.");
	return fmi2Discard;
}

fmi2Status fmi2GetIntegerStatus(fmi2Component c, const fmi2StatusKind s, fmi2Integer* value) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	(void) s;
	(void) value;
	if (invalidState(fmuInstance, "fmi2GetIntegerStatus", not_fmuError)) return fmi2Error;
	fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Discard, "warning", "fmi2GetIntegerStatus: Function not supported.");
	return fmi2Discard;
}

fmi2Status fmi2GetBooleanStatus(fmi2Component c, const fmi2StatusKind s, fmi2Boolean* value) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	(void) s;
	(void) value;
	if (invalidState(fmuInstance, "fmi2GetBooleanStatus", not_fmuError)) return fmi2Error;
	fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Discard, "warning", "fmi2GetBooleanStatus: Function not supported.");
	return fmi2Discard;
}

fmi2Status fmi2GetStringStatus(fmi2Component c, const fmi2StatusKind s, fmi2String* value) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	(void) s;
	(void) value;
	if (invalidState(fmuInstance, "fmi2GetStringStatus", not_fmuError)) return fmi2Error;
	fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Discard, "warning", "fmi2GetStringStatus: Function not supported.");
	return fmi2Discard;
}

fmi2Status fmi2SetRealInputDerivatives(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Integer order[], const fmi2Real value[]) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	(void) vr;
	(void) nvr;
	(void) order;
	(void) value;
	if (invalidState(fmuInstance, "fmi2SetRealInputDerivatives", not_fmuError)) return fmi2Error;
	fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Error, "error", "fmi2SetRealInputDerivatives: Function not supported.");
	return fmi2Error;
}

fmi2Status fmi2GetRealOutputDerivatives(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Integer order[], fmi2Real value[]) {
	FMUInstance* fmuInstance = (FMUInstance *)c;
	(void) vr;
	(void) nvr;
	(void) order;
	(void) value;
	if (invalidState(fmuInstance, "fmi2GetRealOutputDerivatives", not_fmuError)) return fmi2Error;
	fmuInstance->logger(fmuInstance->componentEnvironment, fmuInstance->instanceName, fmi2Error, "error", "fmi2GetRealOutputDerivatives: Function not supported.");
	return fmi2Error;
}

#ifdef __cplusplus
}
#endif // __cplusplus
