#ifndef __SDO_H
#define __SDO_H

#include "stm32f10x.h"
#include "stm32f10x_can.h"
#include "epos.h"

extern CanRxMsg RxMessage;

uint8_t SDO_Write(Epos* epos1,Uint32 Index_Type,Uint8 SubIndex,Uint32 param);
//void SDO_Read(Epos* epos,Uint16 Index,Uint8 SubIndex);
uint32_t SDO_Read(Epos* epos,Uint32 Index_Type,Uint8 SubIndex);

#endif

