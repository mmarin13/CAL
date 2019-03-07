#ifndef HAL_LF_H
#define HAL_LF_H

#include "general_types.h"
#include "mcal_gpio.h"

void LF_vSetPinsDir(BOOL bDir);
void LF_vWritePins(void);
T_U8 LF_u8ReadPins(void);

#endif