/*
 * common.h
 *
 *  Created on: 03-Feb-2016
 *      Author: Agashe
 */

#ifndef SRC_COMMON_H_
#define SRC_COMMON_H_

#include "../headers/B_Timer_Interrupt.h"
#include "../headers/A_MPU6050_I2C.h"
#include "../headers/C_Processing.h"


#define DEFAULT_I2C          I2C0
#define POLLING				 1
#define SPEED_100KHZ         100000		// 100 KHz
#define SPEED_400KHZ         400000		// 400 KHz

#define MPU6050				 0x68       // Slave Address
#define PWR_MGMT_1			 0x6B       // Power Management Register
#define SMPLRT_DIV			 0x19       // Sample Rate Register
#define ACCEL_XOUT_H		 0x3B       // Register Address for AcX High Byte
#define GYRO_XOUT_H			 0x43       // Register Address for GyX High Byte
#define RING_SIZE			 30		// Circular Buffer Size
#define TICKRATE_HZ1 		 4000		// Timer Interrupt Frequency


#define threshold			 0.08
#define buffersize			 2500		// Number of values to be stored and then dumped together



#endif /* SRC_COMMON_H_ */
