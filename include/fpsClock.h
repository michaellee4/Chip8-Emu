#ifndef FPSCLOCK_H
#define FPSCLOCK_H

#include <sys/time.h>
typedef struct
{
	struct timeval start, stop;
}fpsClock;

void fpsStart(fpsClock* fps);

double fpsStop(fpsClock* fps);

#endif

