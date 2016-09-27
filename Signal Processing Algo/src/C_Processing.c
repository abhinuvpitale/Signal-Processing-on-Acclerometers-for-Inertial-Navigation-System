/*
 * C_Processing.c
 *
 *  Created on: 03-Feb-2016
 */
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "board.h"
#include "common.h"


int weights = 0, buffer_index=0, zvc=0, delay=0;

float filter_accel[RING_SIZE][3] = {{0}};  // 2D Array for filtered values
float accelbuff[buffersize] = {}, velbuff[buffersize] = {}, distbuff[buffersize] = {};
float Vx = 0.0, Sx = 0.0;
float OffsetX = 0, OffsetY = 0;

void movement()
{
	weighted_average();								// Averaging (Mean) Filter for Raw IMU data
	/*filter_accel[RING_INDEX][0]=accel[RING_INDEX][0];
	filter_accel[RING_INDEX][1]=accel[RING_INDEX][1];
	filter_accel[RING_INDEX][2]=accel[RING_INDEX][2];*/
	offset();
	RING_INDEX = RING_INDEX + RING_SIZE;	// Note C.0.1
	int moved = 0;

	filter_accel[RING_INDEX % RING_SIZE][0] = filter_accel[RING_INDEX % RING_SIZE][0]/1655;
	filter_accel[RING_INDEX % RING_SIZE][1] = filter_accel[RING_INDEX % RING_SIZE][1]/1655;
	filter_accel[RING_INDEX % RING_SIZE][2] = filter_accel[RING_INDEX % RING_SIZE][2]/1655;

	/*if (fabs(filter_accel[RING_INDEX % RING_SIZE][0]-filter_accel[(RING_INDEX-1) % RING_SIZE][0])>threshold)  // Note C.0.2
		moved = 1;
	else if (fabs(filter_accel[RING_INDEX % RING_SIZE][1]-filter_accel[(RING_INDEX-1) % RING_SIZE][1])>threshold)
		moved = 1;
	else if (fabs(filter_accel[RING_INDEX % RING_SIZE][2]-filter_accel[(RING_INDEX-1) % RING_SIZE][2])>threshold)
		moved = 1; */

	if (fabs(filter_accel[RING_INDEX % RING_SIZE][0])<threshold)
		filter_accel[RING_INDEX % RING_SIZE][0] = 0;

	RING_INDEX = RING_INDEX % RING_SIZE;

	distance();


	if(Sx>=0.5)
	{
		while(1)
		{
			Board_LED_Toggle(0);
			for(delay=0;delay<1000000;delay++);
			Board_LED_Toggle(0);
			for(delay=0;delay<1000000;delay++);
		}
	}

	//printf("\n\r%05f\t%05f\t%05f",filter_accel[RING_INDEX][0],Vx,Sx);


	/*accelbuff[buffer_index] = filter_accel[RING_INDEX%RING_SIZE][0];
	velbuff[buffer_index] = Vx;
	distbuff[buffer_index] = Sx;
	buffer_index+=1;


	if(buffer_index>=buffersize)
	{
		for(buffer_index=0;buffer_index<buffersize;buffer_index++)
		{
			printf("\r%05f\r%05f\r%05f",accelbuff[buffer_index],velbuff[buffer_index],distbuff[buffer_index]);
			printf("\r%05f\t%05f\t%05f\t%d\n",accelbuff[buffer_index],velbuff[buffer_index],distbuff[buffer_index],weights);
		}
		buffer_index=0;
	}*/

}

void offset()
{
	filter_accel[RING_INDEX][0] = filter_accel[RING_INDEX][0] - OffsetX;
	filter_accel[RING_INDEX][1] = filter_accel[RING_INDEX][1] - OffsetY;
	//filter_accel[RING_INDEX][2] = filter_accel[RING_INDEX][2] - OffsetZ;
}

void weighted_average()
{

	int i = 0;
	float sum_x = 0,sum_y = 0,sum_z = 0;
	RING_INDEX = RING_INDEX + RING_SIZE;
	weights = (RING_SIZE*(RING_SIZE+1))/2;
	for(i=0;i<RING_SIZE;i++)
	{
		sum_x = sum_x + ((RING_SIZE-i)*(accel[(RING_INDEX-i)%RING_SIZE][0]));
		sum_y = sum_y + ((RING_SIZE-i)*(accel[(RING_INDEX-i)%RING_SIZE][1]));
		sum_z = sum_z + ((RING_SIZE-i)*(accel[(RING_INDEX-i)%RING_SIZE][2]));
	}

	RING_INDEX = RING_INDEX % RING_SIZE;

	filter_accel[RING_INDEX][0] = sum_x/(weights);
	filter_accel[RING_INDEX][1] = sum_y/(weights);
	filter_accel[RING_INDEX][2] = sum_z/(weights);
}

void distance()
{
	Sx = Sx + Vx*(time) + 0.5*(filter_accel[RING_INDEX][0])*(time)*(time);
	Vx = Vx + filter_accel[RING_INDEX][0]*(time);
	if(filter_accel[RING_INDEX][0]==0)
	{
		zvc++;
		if(zvc>=40)
		{
			Vx=0;
			zvc=0;
		}
	}
	else
		zvc=0;
}

void dynamic_offset()
{
			OffsetX = OffsetX + (accel[RING_INDEX][0]/500);
			OffsetY = OffsetY + (accel[RING_INDEX][1]/500);
}

void print_values()
{
	printf("\r\n AcX= %05f\tAcY= %05f\tAcZ= %05f GyX= %05f\tGyY= %05f\tGyZ= %05f",accel[RING_INDEX][0],accel[RING_INDEX][1],accel[RING_INDEX][2],gyro[RING_INDEX][0],gyro[RING_INDEX][1],gyro[RING_INDEX][2]);
}
