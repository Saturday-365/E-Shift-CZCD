#ifndef __SA_KEY_H
#define __SA_KEY_H
#include "gpio.h"
#include "main.h"

#define KEY_NUMBER 5
#define KEY_LIST                                                               \
  { DOWNSHIFTSIG, UPSHIFTSIG, SET_N, SWITCH2, SWITCH3}

#define KEY_RELEASE_LEVEL                                                      \
  (GPIO_PIN_RESET) // 按键的默认状态 也就是按键释放状态的电平
#define KEY_MAX_SHOCK_PERIOD                                                   \
  (10) // 按键消抖检测时长 单位毫秒 低于这个时长的信号会被认为是杂波抖动
#define KEY_LONG_PRESS_PERIOD                                                  \
  (450) // 最小长按时长 单位毫秒 高于这个时长的信号会被认为是长按动作
#define With_Tremble // 按键硬件电路是否含有消抖电路 With_Tremble/Without_Tremble

typedef enum {
  DOWNSHIFTSIG,
  UPSHIFTSIG,
  SET_N,
  SWITCH2,
  SWITCH3,
} key_index_enum; // 按键索引 对应上方定义的按键引脚个数 默认定义四个按键

typedef enum {
    KEY_RELEASE = 0,           // 释放状态
    KEY_PRESSED,          // 刚刚按下（新状态）
    KEY_LONG_PRESS             // 长按状态
} key_state_enum;

void key_scanner(void);
key_state_enum key_get_state(key_index_enum key_n);
void key_clear_state(key_index_enum key_n);
void key_clear_all_state(void);
void key_init(uint32_t period);




#endif
