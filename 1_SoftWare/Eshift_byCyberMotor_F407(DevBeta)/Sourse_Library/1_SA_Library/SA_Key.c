#include "main.h"
#include "SA_Key.h"
#include "gpio.h"

static uint32_t             scanner_period = 1;                                 // 按键的扫描周期
static key_index_enum       key_index[KEY_NUMBER] = KEY_LIST;
static uint32_t             key_press_time[KEY_NUMBER];                         // 按键信号持续时长
static uint32_t             key_stable_time[KEY_NUMBER];                        // 按键稳定状态时长
static key_state_enum       key_state[KEY_NUMBER];                              // 按键状态
static GPIO_PinState        last_level[KEY_NUMBER];                             // 上一次检测的电平

GPIO_PinState gpio_get_level(key_index_enum key_index){
    switch (key_index)
		{
		case DOWNSHIFTSIG :return HAL_GPIO_ReadPin(DOWNSHIFTSIG_IN_GPIO_Port, DOWNSHIFTSIG_IN_Pin);
		case UPSHIFTSIG:return HAL_GPIO_ReadPin(UPSHIFTSIG_IN_GPIO_Port, UPSHIFTSIG_IN_Pin);
		case SET_N:return HAL_GPIO_ReadPin(SWITCH1_GPIO_Port, SWITCH1_Pin);
		case SWITCH2 :return HAL_GPIO_ReadPin(SWITCH2_GPIO_Port, SWITCH2_Pin);
		case SWITCH3:return HAL_GPIO_ReadPin(SWITCH3_GPIO_Port, SWITCH3_Pin);
		default:return GPIO_PIN_SET;
		} 
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     按键状态扫描
// 参数说明     void
// 返回参数     void
// 使用示例     key_scanner();
// 备注信息     这个函数放在主循环或者定时器中断中
//-------------------------------------------------------------------------------------------------------------------
void key_scanner(void)
{
    uint8_t i = 0;
    GPIO_PinState current_level;
    
    for(i = 0; i < KEY_NUMBER; i++)
    {
        current_level = gpio_get_level(key_index[i]);
        
        // 检测到电平变化，重置稳定计时器
        if(current_level != last_level[i])
        {
            key_stable_time[i] = 0;
            last_level[i] = current_level;
        }
        else
        {
            // 电平稳定，增加稳定时间
            key_stable_time[i]++;
        }
        
        // 电平稳定时间超过消抖阈值
        if(key_stable_time[i] >= KEY_MAX_SHOCK_PERIOD / scanner_period)
        {
            if(KEY_RELEASE_LEVEL != current_level)  // 按键按下状态
            {
                key_press_time[i]++;
                
                // 刚按下状态 - 提高短按响应速度
                if(key_press_time[i] == 1)
                {
                    key_state[i] = KEY_JUST_PRESSED;  // 新增状态：刚刚按下
                }
                // 判断长按
                else if(key_press_time[i] >= KEY_LONG_PRESS_PERIOD / scanner_period)
                {
                    key_state[i] = KEY_LONG_PRESS;
                }
                // 短按保持状态
                else
                {
                    if(key_state[i] != KEY_LONG_PRESS)
                    {
                        key_state[i] = KEY_SHORT_PRESS_HOLD;  // 新增状态：短按保持
                    }
                }
            }
            else  // 按键释放状态
            {
                // 如果之前是短按或长按状态，释放时标记为已释放
                if(key_state[i] == KEY_SHORT_PRESS_HOLD || 
                   key_state[i] == KEY_JUST_PRESSED ||
                   key_state[i] == KEY_LONG_PRESS)
                {
                    // 如果按下时间较短且不是长按，则标记为短按释放
                    if(key_press_time[i] < KEY_LONG_PRESS_PERIOD / scanner_period)
                    {
                        key_state[i] = KEY_SHORT_PRESS_RELEASE;  // 新增状态：短按释放
                    }
                    else
                    {
                        key_state[i] = KEY_RELEASE;
                    }
                }
//                else
//                {
//                    key_state[i] = KEY_RELEASE;
//                }
                key_press_time[i] = 0;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取按键状态
// 参数说明     key_n           按键索引
// 返回参数     key_state_enum  按键状态
// 使用示例     key_get_state(KEY_1);
//-------------------------------------------------------------------------------------------------------------------
key_state_enum key_get_state(key_index_enum key_n)
{
    return key_state[key_n];
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     清除对应按键状态
// 参数说明     key_n           按键索引
// 返回参数     void            无
// 使用示例     key_clear_state(KEY_1);
//-------------------------------------------------------------------------------------------------------------------
void key_clear_state(key_index_enum key_n)
{
    // 对于短按释放和刚刚按下状态，直接清除
    if(key_state[key_n] == KEY_SHORT_PRESS_RELEASE || 
       key_state[key_n] == KEY_JUST_PRESSED)
    {
        key_state[key_n] = KEY_RELEASE;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     清除所有按键状态
// 参数说明     void            无
// 返回参数     void            无
// 使用示例     key_clear_all_state();
//-------------------------------------------------------------------------------------------------------------------
void key_clear_all_state(void)
{
    for(uint8_t i = 0; i < KEY_NUMBER; i++)
    {
        key_clear_state((key_index_enum)i);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     按键初始化
// 参数说明     period          按键扫描周期 以毫秒为单位
// 返回参数     void
// 使用示例     key_init(10);
//-------------------------------------------------------------------------------------------------------------------
void key_init(uint32_t period)
{
    scanner_period = period;
    for(uint8_t i = 0; i < KEY_NUMBER; i++)
    {
        key_state[i] = KEY_RELEASE;
        key_press_time[i] = 0;
        key_stable_time[i] = 0;
        last_level[i] = gpio_get_level(key_index[i]);
    }
}
