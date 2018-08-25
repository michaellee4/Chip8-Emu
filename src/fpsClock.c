#include <stdlib.h>
#include "fpsClock.h"

void fpsStart(fpsClock* fps)
{
	gettimeofday(&fps->start, NULL);
}

double fpsStop(fpsClock* fps)
{
	gettimeofday(&fps->stop, NULL);
	return (fps->stop.tv_sec- fps->start.tv_sec) * 1000 + (fps->stop.tv_usec - fps->start.tv_usec) / 1000;
}


