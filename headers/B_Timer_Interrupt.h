/*
 * Timer_Interrupt.h
 *
 *  Created on: 28-Jan-2016
 */

#ifndef HEADERS_B_TIMER_INTERRUPT_H_
#define HEADERS_B_TIMER_INTERRUPT_H_

extern float time;

void TIMER0_IRQHandler(void);
int Interrupt_Setup(void);

#endif /* HEADERS_B_TIMER_INTERRUPT_H_ */
