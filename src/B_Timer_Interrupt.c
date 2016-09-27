/*
 * B_Timer_Interrupt.c
 *
 *  Created on: 03-Feb-2016
 */
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "common.h"

#define TICKRATE_HZ1 		 4000	/* 11 ticks per second */

float time, a_time;
int i=0;

void TIMER0_IRQHandler(void)
{
	if (Chip_TIMER_MatchPending(LPC_TIMER0, 1)) {
		Chip_TIMER_ClearMatch(LPC_TIMER0, 1);
		/*if (i == (TICKRATE_HZ1/4))
		{

			i=0;
			//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("\n\ra_time = %f",a_time);
		}*/
		Board_LED_Toggle(0);
		i++;
		a_time += time;
		Get_IMU_Values();						// Function called at every timer interrupt


	}
}

int Interrupt_Setup(void)
{
	uint32_t timerFreq;

	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Enable timer 1 clock */
	Chip_TIMER_Init(LPC_TIMER0);

	/* Timer rate is system clock rate */
	timerFreq = Chip_Clock_GetSystemClockRate();

	/* Timer setup for match and interrupt at TICKRATE_HZ */
	Chip_TIMER_Reset(LPC_TIMER0);
	Chip_TIMER_MatchEnableInt(LPC_TIMER0, 1);
	Chip_TIMER_SetMatch(LPC_TIMER0, 1, (timerFreq / TICKRATE_HZ1));
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER0, 1);
	Chip_TIMER_Enable(LPC_TIMER0);
	time = 4.0/(TICKRATE_HZ1);

	/* Enable timer interrupt */
	NVIC_ClearPendingIRQ(TIMER0_IRQn);
	NVIC_EnableIRQ(TIMER0_IRQn);
	return 0;
}
