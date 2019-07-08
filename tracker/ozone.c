#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <wiringPi.h>
#include <mcp3004.h>

#include "gps.h"
#include "ozone.h"
#include "misc.h"

double ReadOzone(int chan)
{
	int RawValueOzone;
	int RawValueNO2;
	double ResultMRsv;
	double ResultRP;
	double ResultMRrv;

   	RawValueOzone = analogRead(chan);
	RawValueNO2 = analogRead(chan);
	ResultRP = ((double)RawValueOzone-(double)RawValueNO2) / 1024.0 * 5.0;
	
		//Calibration missing
	//printf("RawValue = %i\n", RawValue);

	return ResultRP;
	
}


void *OzoneLoop(void *some_void_ptr)
{
	double Ozone;
	FILE* fp;
	struct TGPS *GPS;

	GPS = (struct TGPS *)some_void_ptr;

	if (mcp3004Setup(100, 0)<0)
	{
		printf("failed to setup SPI");
	}
		
	while (1)
	{
		Ozone = ReadOzone(100);
		GPS->OzonePP = Ozone ;

		// printf("Ozone = %lf\n", Ozone);

		sleep(10);
	}

	return 0;
}
