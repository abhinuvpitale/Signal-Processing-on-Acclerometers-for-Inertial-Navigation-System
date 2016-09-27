/*
 * A_MPU6050_I2C.h
 *
 *  Created on: 28-Jan-2016
 */

#ifndef HEADERS_A_MPU6050_I2C_H_
#define HEADERS_A_MPU6050_I2C_H_

extern int accel[][3];	// Variables declared as 'external' to
extern int gyro[][3];	// use in other C files in the project
extern int RING_INDEX;

/**--------------------------------- A.0 ------------------------------------//
 * @brief	Sets the I2C Mode of operation
 * @param	id				: I2C peripheral ID (I2C0, I2C1 ... etc)
 * @param   polling 		: Polling = 1 Sets I2C mode of operation to polling
 */void i2c_set_mode(I2C_ID_T id, int polling);


 /**--------------------------------- A.1 ------------------------------------//
 * @brief	Initialize I2C
 * @param	id				: I2C peripheral ID (I2C0, I2C1 ... etc)
 * @param 	speed			: Speed of I2C Communication
 */void i2c_app_init(I2C_ID_T id, int speed);


 /**--------------------------------- A.2 ------------------------------------//
 * @brief	Function is called in Interrupt Handler(3.1) to get values
 */void Get_IMU_Values();

 /**--------------------------------- A.3 ------------------------------------//
  * @brief	Sets up the MPU6050
  */void IMU_Setup();

 /**--------------------------------- A.4 ------------------------------------//
 * @brief	Data is converted from 2's complement format to Decimal
 * @param	Reg_Addr		: Register Address of Slave
 * @param	format_buffer	: Address of buffer containing data to be formatted
 * @note 	A.4.0			: High Byte left shifted by 8 bits and OR with Low Byte
 * @note	A.4.1			: 16-bit value right shifted 15 bits to get MSB(sign bit)
 * @note	A.4.2			: Conversion from 2's complement format to decimal
 */void format_data(int Reg_Addr, uint8_t format_buffer[]);

 /**--------------------------------- A.5 ------------------------------------//
 * @brief	Write Data to Slave using I2C Protocol
 * @param	Reg_Addr		: Register Address of Slave
 * @param	Data			: Data byte to be written
 */void i2c_WRITE(int Reg_Addr,int Data);

 /**--------------------------------- A.6 ------------------------------------//
  * @brief	Write Data to Slave using I2C Protocol
  * @param	Reg_Addr		: Register Address of Slave
  * @param	read_buffer		: Address of buffer that will hold data read from Slave
  */void i2c_READ(int Reg_Addr,uint8_t read_buffer[]);

#endif /* HEADERS_A_MPU6050_I2C_H_ */
