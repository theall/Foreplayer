#ifndef UTILS_H
#define UTILS_H

#include "../../../thirdparty/m1lib/src/m1ui.h"

// call this on startup to initialize the core.
typedef void (*M1SND_INIT)(void *, int (*m1ui_message)(void *,int, char *, int));

// call this for "running" messages.
typedef int (*M1SND_RUN)(int command, int iparm);

// call this at program shutdown to shut down the core.
typedef void (*M1SND_SHUTDOWN)(void);

// call to set core options
typedef void (*M1SND_SETOPTION)(int option, int value);

// call to get integer information from the core
typedef int (*M1SND_GET_INFO_INT)(int iinfo, int parm);

// call to get string information from the core
typedef char *(*M1SND_GET_INFO_STR)(int sinfo, int parm);

// call to get extended string info
typedef char *(*M1SND_GET_INFO_STR_EX)(int sinfo, int parm1, int parm2, int parm3);

// call to set string data
typedef void (*M1SND_SET_INFO_STR)(int sinfo, char *info, int parm1, int parm2, int parm3);

// call to set integer information in the core
typedef void (*M1SND_SET_INFO_INT)(int iinfo, int parm1, int parm2, int parm3);

// call when not using M1's internal sound driver to create a frame of output
typedef void (*M1SND_DO_FRAME)(unsigned long dwSamples, signed short *out);

// call when lang is changed
typedef void (*M1SND_SWITCHLANG)(int lang);

extern M1SND_INIT proc_init;
extern M1SND_RUN proc_run;
extern M1SND_SHUTDOWN proc_shutdown;
extern M1SND_SETOPTION proc_setoption;
extern M1SND_GET_INFO_INT proc_get_info_int;
extern M1SND_GET_INFO_STR proc_get_info_str;
extern M1SND_GET_INFO_STR_EX proc_get_info_str_ex;
extern M1SND_SET_INFO_STR proc_set_info_str;
extern M1SND_SET_INFO_INT proc_set_info_int;
extern M1SND_DO_FRAME proc_do_frame;
extern M1SND_SWITCHLANG proc_switchlang;

int m1ui_message(void *user, int message, char *txt, int iparam);

#endif // UTILS_H

