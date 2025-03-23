
#ifndef __COMMON_H_
#define __COMMON_H_
#include <string.h>
#include <stdio.h>

typedef unsigned char   uint8  ;	//  8 bits 
typedef unsigned int  	uint16 ;	// 16 bits 
typedef unsigned long  	uint32 ;	// 32 bits 
								
								
typedef signed char     int8   ;	//  8 bits 
typedef signed int      int16  ;	// 16 bits 
typedef signed long     int32  ;	// 32 bits 
								
								
typedef volatile int8   vint8  ;	//  8 bits 
typedef volatile int16  vint16 ;	// 16 bits 
typedef volatile int32  vint32 ;	// 32 bits 
								
								
typedef volatile uint8  vuint8 ;	//  8 bits 
typedef volatile uint16 vuint16;	// 16 bits 
typedef volatile uint32 vuint32;	// 32 bits 



typedef enum 
{
    NO_WIRELESS_MODE = 0,   
    WIRELESS_SI24R1 = 1,    
    WIRELESS_CH9141 = 2,    
    WIRELESS_CH573 = 3      
}WIRELESS_TYPE_enum;

extern WIRELESS_TYPE_enum wireless_type;




#endif
