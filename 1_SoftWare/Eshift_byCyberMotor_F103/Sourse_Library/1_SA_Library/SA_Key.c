#include "main.h"

uint16_t get_key_num(){

if    (HAL_GPIO_ReadPin(SWITCH2_GPIO_Port, SWITCH2_Pin)) return 4;
if    (HAL_GPIO_ReadPin(SWITCH1_GPIO_Port, SWITCH1_Pin)) return 3;  
if    (HAL_GPIO_ReadPin(UPSHIFTSIG_IN_GPIO_Port, UPSHIFTSIG_IN_Pin)) return 1;        
if    (HAL_GPIO_ReadPin(DOWNSHIFTSIG_IN_GPIO_Port, DOWNSHIFTSIG_IN_Pin)) return 2;         
else return 0;
}
