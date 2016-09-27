/*
 * A_MPU6050_I2C.c
 *
 *  Created on: 03-Feb-2016
 */
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "common.h"

int mode_poll;  						 // Poll/Interrupt mode flag
uint8_t buffer_TX[2];
uint8_t accel_buffer[6];         // Buffer to store values received from slave
uint8_t gyro_buffer[6];          // Buffer to store values received from slave
int accel[RING_SIZE][3];         // Array for Processed Accel Values
int gyro[RING_SIZE][3];          // Array for Processed Gyro Values
int detect_movement = 0;         // Flag for movement detection
int bytes_read =2;               // Value returned my Chip_I2C_MasterCmdRead
int RING_INDEX = 0;              // Index Value for 2-D Ring Buffer 
int offset_counter = 0;			 // counter for measuring dynamic offset

/* Set I2C mode to polling/interrupt */
void i2c_set_mode(I2C_ID_T id, int polling)
{
	if(!polling) {
		mode_poll &= ~(1 << id);
		Chip_I2C_SetMasterEventHandler(id, Chip_I2C_EventHandler);
		NVIC_EnableIRQ(id == I2C0 ? I2C0_IRQn : I2C1_IRQn);
	} else {
		mode_poll |= 1 << id;
		NVIC_DisableIRQ(id == I2C0 ? I2C0_IRQn : I2C1_IRQn);
		Chip_I2C_SetMasterEventHandler(id, Chip_I2C_EventHandlerPolling);
	}
}

/* Initialize the I2C bus */
void i2c_app_init(I2C_ID_T id, int speed)
{
	Board_I2C_Init(id);
	/* Initialize I2C */
	Chip_I2C_Init(id);
	Chip_I2C_SetClockRate(id, speed);
	/* Set default mode to interrupt */
	i2c_set_mode(id, 0);
}

/* Function is Called in Interrupt Handler */
void Get_IMU_Values()
{
	i2c_READ(ACCEL_XOUT_H ,accel_buffer); 	 // Receive 6 bytes of Accel Data
	i2c_READ(GYRO_XOUT_H  ,gyro_buffer );	 // Receive 6 bytes of Gyro Data

	if(offset_counter < 500)
	{
		dynamic_offset();
		offset_counter++;
	}

	else if (detect_movement == TRUE && offset_counter >= 500)
		movement();
	else
		print_values();


	if (bytes_read != 0) RING_INDEX++;       // Increment RING_INDEX
		RING_INDEX = RING_INDEX % RING_SIZE; // Wrap around for Ring Buffer
}

void IMU_Setup()
{
	i2c_app_init(I2C0, SPEED_400KHZ);		 // Set Communication Freq = 400 KHz
	i2c_set_mode(I2C0, POLLING);
	i2c_WRITE(PWR_MGMT_1 ,0);		 	     // Wake up MPU6050
	i2c_WRITE(SMPLRT_DIV ,7);			 	 // Set Sampling Rate to 1 kHz
}

/* Refer MPU6050_I2C.h for function description */
void format_data(int Reg_Addr, uint8_t format_buffer[])
{
	int i=0;
	int value, sign_bit;
	while(i<6)
		{
		value = format_buffer[i] << 8 | format_buffer[i+1]; // Note A.4.0
		sign_bit = value >> 15;								// Note A.4.1
		if(sign_bit == 1)
			{
			value = value^0xFFFF;							// Note A.4.2
			value = (-1)*(value+1);
			}
		if (Reg_Addr == ACCEL_XOUT_H)
			accel[RING_INDEX][i/2] = value;			// Store Formatted Decimal Values
		else										// into accel / gyro 2D Array
			gyro[RING_INDEX][i/2] = value;
		i=i+2;
		}
}

/* Data Write */
void i2c_WRITE(int Reg_Addr,int Data)
{
	int size;
	I2C_XFER_T xfer;						// Structure Variable
	xfer.slaveAddr = MPU6050;				// Assigning Slave Address
	buffer_TX[0]= Reg_Addr;					// buffer_TX contains data to be
	buffer_TX[1]= Data;						// sent to slave
	size = sizeof(buffer_TX);
	xfer.txSz = 2;							// 2 bytes to be sent(Reg_Addr and Data)
	xfer.txBuff = &buffer_TX[0];			// I2C txBuff assigned address of buffer_TX
	Chip_I2C_MasterSend(I2C0, xfer.slaveAddr, xfer.txBuff, xfer.txSz);
}
/* Data Read */
void i2c_READ(int Reg_Addr,uint8_t read_buffer[])
{
	I2C_XFER_T xfer;
	xfer.slaveAddr = MPU6050;				// Assigning Slave Address
	xfer.rxSz = 6; 							// Read 6 bytes
	xfer.rxBuff = &read_buffer[0];			// I2C txBuff assigned address of read_buffer
	bytes_read = Chip_I2C_MasterCmdRead(I2C0, xfer.slaveAddr, Reg_Addr, xfer.rxBuff, xfer.rxSz);
	format_data(Reg_Addr , read_buffer);	// Formatting data from 2's complement
}											// format to decimal format
