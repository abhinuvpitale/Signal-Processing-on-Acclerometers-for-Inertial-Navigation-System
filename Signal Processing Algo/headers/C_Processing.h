/*
 * Data_Processing.h
 *
 *  Created on: 29-Jan-2016
 */

#ifndef HEADERS_C_PROCESSING_H_
#define HEADERS_C_PROCESSING_H_

/**--------------------------------- C.0 ------------------------------------//
 * @brief	Detect movement by comparing successive Accelerometer Readings
 * @note	C.0.1		RING_INDEX is up-scaled by RING_SIZE (refer notes.txt)
 * @note	C.0.2		Successive Values are Subtracted and Compared with a set Threshold
 */void movement();

/**--------------------------------- C.1 ------------------------------------//
 * @brief	Print Data to Console
 */void print_values();

/**--------------------------------- C.2 ------------------------------------//
 * @brief	Moving Average (Mean) of Raw IMU data
 */void weighted_average();

/**--------------------------------- C.3 ------------------------------------//
 * @brief	Calculate displacement
 */ void distance();

/**--------------------------------- C.4 ------------------------------------//
 * @brief	Subtracts Offset Values from
 */void offset();
#endif /* HEADERS_C_PROCESSING_H_ */
