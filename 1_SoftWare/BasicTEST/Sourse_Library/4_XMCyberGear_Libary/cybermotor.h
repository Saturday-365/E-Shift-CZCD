#ifndef __CYBERMOTOR_H
#define __CYBERMOTOR_H
 
#include "cybergear.h"
#include <math.h>
//该文件应该包括 绝对零点设置；正弦运动设置；
#ifdef __cplusplus
extern "C" {
#endif
    
/*****************************获取期望零位*****************************/
void Init_CyberZero(Cyber_Motor *Motor);
void Init_Sin(Cyber_Motor *Motor);
 
void Setting_AbsoluteZero(Cyber_Motor *Motor);
void Motion_CyberSin(Cyber_Motor *Motor);
    
#ifdef __cplusplus
}
#endif
#endif

