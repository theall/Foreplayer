//
// Audio Overload SDK
//
// Portability wrappers.
//

#include <stdio.h>

#ifdef WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

FILE* ao_fopen(const char *fn, const char *mode)
{
	return fopen(fn, mode);
}

int ao_mkdir(const char *dirname)
{
    (void)dirname;
#ifdef WIN32
	// Let's avoid the entire problem of certain runtimes omitting the
	// mode parameter altogether by just using the kernel function.
    return 0;//CreateDirectoryA(dirname, NULL);
#else
	return mkdir(dirname, 0777);
#endif
}

void ao_sleep(unsigned int msecs)
{
    (void)msecs;
#ifdef ENABLE_PLAY
    usleep((useconds_t)(msecs) * 1000);
#endif
}
