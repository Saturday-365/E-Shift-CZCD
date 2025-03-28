#include "pc_communication.h"
 
volatile float motor_info[5];//不能设置为uint_8，必须要和实际的Motor结构体的一致，否则无法发送
#ifdef __cplusplus
extern "C" {
#endif
    
void PC_Communication(UART_HandleTypeDef* huart, const char* format, ...) {
    static char buffer[1000];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    HAL_UART_Transmit_DMA(huart, (uint8_t*)buffer, strlen(buffer));
}
 
void curve_Display(float a, float b, float c, float d)
{
    PC_Communication(&huart1, "d: %.2f, %.2f, %.2f, %.2f\r\n", a, b, c, d);
}
 
#ifdef __cplusplus

#endif
