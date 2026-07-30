/**
 * @file     softwareiic.c
 * @brief    适用于STM32 HAL库及CUBEMX生成工程的软件模拟IIC
 * @version  V1.0.0
 * @author   Sundea
 * @date     2023/5/8
 */
#include "SA_SoftIIC.h"

/**
 * @brief IIC延时
 * @param  无
 * @return 无
 */
void IIC_Delay(uint8_t time)
{
    uint32_t i = time * 800; // 用户根据自己的MCU时钟进行设置倍数

    while (i--)
    {
        ;
    }
}

/**
 * @brief IIC初始化
 * @param  无
 * @return 无
 */
void Soft_IIC_Init(void)
{
    IIC_SDA_H();
    IIC_SCL_H();
}

/**
 * @brief SDA引脚设置输出模式
 * @param  无
 * @return 无
 */
static void Soft_IIC_Output(void)
{
    GPIO_InitTypeDef SOFT_IIC_GPIO_STRUCT;
    SOFT_IIC_GPIO_STRUCT.Mode = GPIO_MODE_OUTPUT_PP;
    SOFT_IIC_GPIO_STRUCT.Pin = IIC_SDA_PIN;
    SOFT_IIC_GPIO_STRUCT.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(IIC_SDA_PORT, &SOFT_IIC_GPIO_STRUCT);
}

/**
 * @brief SDA引脚设置输入模式
 * @param  无
 * @return 无
 */
static void Soft_IIC_Input(void)
{
    GPIO_InitTypeDef SOFT_IIC_GPIO_STRUCT;
    SOFT_IIC_GPIO_STRUCT.Mode = GPIO_MODE_INPUT;
    SOFT_IIC_GPIO_STRUCT.Pin = IIC_SDA_PIN;
    SOFT_IIC_GPIO_STRUCT.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(IIC_SDA_PORT, &SOFT_IIC_GPIO_STRUCT);
}

/**
 * @brief IIC起始信号
 * @param  无
 * @return 无
 */
void Soft_IIC_Start(void)
{
    Soft_IIC_Output();
    IIC_SCL_L();
    IIC_SDA_H();
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SDA_L();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_L();
}

/**
 * @brief IIC停止信号
 * @param  无
 * @return 无
 */
void Soft_IIC_Stop(void)
{
    Soft_IIC_Output();
    IIC_SCL_L();
    IIC_SDA_L();
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SDA_H();
    IIC_Delay(IIC_DELAY_TIME);
}

/**
 * @brief IIC应答信号
 * @param  无
 * @return 无
 */
void Soft_IIC_ACK(void)
{
    Soft_IIC_Output();
    IIC_SCL_L();
    IIC_SDA_L();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_L();
}

/**
 * @brief IIC无应答信号
 * @param  无
 * @return 无
 */
void Soft_IIC_NACK(void)
{
    Soft_IIC_Output();
    IIC_SCL_L();
    IIC_SDA_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
}

/**
 * @brief IIC等待应答信号
 * @param  无
 * @return 0无应答  1有应答
 */
uint8_t Soft_IIC_Wait_ACK(void)
{
    uint8_t wait;
    Soft_IIC_Output();
    IIC_SDA_H();
    Soft_IIC_Input();
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    while (HAL_GPIO_ReadPin(IIC_SDA_PORT, IIC_SDA_PIN))
    {
        wait++;
        if (wait > 200)
        {
            Soft_IIC_Stop();
            return 0;
        }
    }
    IIC_SCL_L();
    return 1;
}

/**
 * @brief IIC写数据1
 * @param  无
 * @return 无
 */
void Soft_IIC_Write_High(void)
{
    IIC_SCL_L();
    IIC_SDA_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_L();
}

/**
 * @brief IIC写数据0
 * @param  无
 * @return 无
 */
void Soft_IIC_Write_Low(void)
{
    IIC_SCL_L();
    IIC_SDA_L();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    IIC_SCL_L();
}

/**
 * @brief IIC写入单个数据
 * @param  无
 * @return 应答信号, 0无应答 1有应答
 */
uint8_t Soft_IIC_Write_Byte(uint8_t Byte)
{
    uint8_t i;
    Soft_IIC_Output();
    for (i = 0x80; i != 0; i >>= 1)
    {
        if (Byte & i)
        {
            Soft_IIC_Write_High();
        }
        else
        {
            Soft_IIC_Write_Low();
        }
    }
    return (Soft_IIC_Wait_ACK());
}

/**
 * @brief IIC读一个数据
 * @param  ACK:应答 NACK:不应答
 * @return 返回读到的数据
 */
uint8_t Soft_IIC_Recv_Byte(ACK_STATUS ack_sta)
{
    uint8_t data = 0, i;
    Soft_IIC_Input();
    IIC_SCL_H();
    IIC_Delay(IIC_DELAY_TIME);
    for (i = 0x80; i != 0; i >>= 1)
    {
        if (HAL_GPIO_ReadPin(IIC_SDA_PORT, IIC_SDA_PIN) == 1)
        {
            data |= i;
        }
        IIC_Delay(IIC_DELAY_TIME);
        IIC_SCL_L();
        IIC_Delay(IIC_DELAY_TIME);
        IIC_SCL_H();
        IIC_Delay(IIC_DELAY_TIME);
    }
    if (ack_sta == ACK)
    {
        Soft_IIC_ACK();
    }
    else
    {
        Soft_IIC_NACK();
    }
    return data;
}
