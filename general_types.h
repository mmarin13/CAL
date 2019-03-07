/* 
 * File:   general_types.h
 * Author: Cristian T. A.
 *
 * Created on August 1, 2018, 11:13 AM
 */

#ifndef GENERAL_TYPES_H
#define	GENERAL_TYPES_H

typedef unsigned char   T_U8;
typedef char            T_S8;
typedef unsigned short  T_U16;
typedef short           T_S16;
typedef float           T_F16;
typedef enum _BOOL{FALSE=0, TRUE=1, UNDEFINED=2} BOOL; 

typedef struct carInfo
{
    T_U8 u8RoadNum;
    T_U8 u8Direction;
    BOOL bIsOnMain;
}carInfo; 

#endif	/* GENERAL_TYPES_H */

