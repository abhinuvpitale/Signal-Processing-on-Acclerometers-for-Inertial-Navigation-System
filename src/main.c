#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "common.h"

#define TRUE	1
#define FALSE	0

#ifdef DEBUG_ENABLE
#endif
extern int detect_movement;
int main(void)
{
	SystemCoreClockUpdate();
	Board_Init();
	IMU_Setup();
	Interrupt_Setup();
	detect_movement = TRUE;
	Chip_I2C_DeInit(I2C0);

	return 0;
}
