#ifndef __SA_KEY_H
#define __SA_KEY_H
#include "gpio.h"
#include "main.h"

#define KEY_NUMBER 5
#define KEY_LIST                                                               \
  { DOWNSHIFTSIG, UPSHIFTSIG, SET_N, SWITCH2, SWITCH3}

#define KEY_RELEASE_LEVEL                                                      \
  (GPIO_PIN_SET) // 按键的默认状态 也就是按键释放状态的电平
#define KEY_MAX_SHOCK_PERIOD                                                   \
  (5) // 按键消抖检测时长 单位毫秒 低于这个时长的信号会被认为是杂波抖动
#define KEY_LONG_PRESS_PERIOD                                                  \
  (300) // 最小长按时长 单位毫秒 高于这个时长的信号会被认为是长按动作

typedef enum {
  DOWNSHIFTSIG,
  UPSHIFTSIG,
  SET_N,
  SWITCH2,
  SWITCH3,
} key_index_enum; // 按键索引 对应上方定义的按键引脚个数 默认定义四个按键

typedef enum {
    KEY_RELEASE = 0,           // 释放状态
    KEY_JUST_PRESSED,          // 刚刚按下（新状态）
    KEY_SHORT_PRESS_HOLD,      // 短按保持（新状态）
    KEY_SHORT_PRESS_RELEASE,   // 短按释放（新状态）
    KEY_LONG_PRESS             // 长按状态
} key_state_enum;

void key_scanner(void);
key_state_enum key_get_state(key_index_enum key_n);
void key_clear_state(key_index_enum key_n);
void key_clear_all_state(void);
void key_init(uint32_t period);




#endif
