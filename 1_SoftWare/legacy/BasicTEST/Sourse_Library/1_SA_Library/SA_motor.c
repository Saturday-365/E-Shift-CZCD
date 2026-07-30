#include "math.h"
//#include "i2c.h"
#include "stdio.h"
#include "string.h"
#include "MT6701.h"
#include "SA_Delay.h"

#include "tim.h"
#include "SA_motor.h"

float voltage_power_supply = 12;
unsigned int cvalue=10000;

//    void HAL_SETPWM_init(void)
//    {
//    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);

//    }
//    void M1_HAL_SETPWM(float a1)
//    {   
//    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, a1*cvalue); //设置CH4->PWM脉冲宽度    
//    }

//    // 归一化角度到 [0,2PI]


//    float normalizeAngle(float angle) {
//    float a = fmod(angle, 2 * PI);  //取余运算可以用于归一化，列出特殊值例子算便知
//    return a >= 0 ? a : (a + 2 * PI);
//    //fmod 函数的余数的符号与除数相同。因此，当 angle 的值为负数时，余数的符号将与 _2PI 的符号相反。也就是说，如果 angle 的值小于 0 且 _2PI 的值为正数，则 fmod(angle, _2PI) 的余数将为负数。
//    //例如，当 angle 的值为 -PI/2，_2PI 的值为 2PI 时，fmod(angle, _2PI) 将返回一个负数。在这种情况下，可以通过将负数的余数加上 _2PI 来将角度归一化到 [0, 2PI] 的范围内，以确保角度的值始终为正数。
//    }

//    void FOCType_Init(FOCmotor_Typedef*foc,int _PP, int _DIR,char _ch) //结构体参数定义
//    {
//    foc->ch=_ch;  //电机通道    
//    foc->PP = _PP;  //极对数
//    foc->DIR = _DIR;  //方向
//    foc->zero_electric_angle=0;
//    foc->Ua=0;
//    foc->dc_a=0;
//    foc->Ub=0;
//    foc->dc_b=0;
//    foc->Uc=0;
//    foc->dc_c=0;
//    foc->Uq=0;
//    foc->angle_el=0;
//    foc->T0=0;
//    foc->T1=0;
//    foc->T2=0;
//    foc->Ta=0;
//    foc->Tb=0;
//    foc->Tc=0;
//    foc->sector=0;   
//    }  

//    void FOCType_setfirst(FOCmotor_Typedef*foc) //结构体参数定义
//    {
//    foc->zero_electric_angle=_3PI_2;
//    } 

//    float electricalAngle(int DIR,int PP,float zero_electric_angle,float anglein) {

//    return normalizeAngle((float)(DIR * PP)*anglein  - zero_electric_angle);
//    }

//    //void Get_ALL_zeroangle(FOCmotor_Typedef*foc1,FOCmotor_Typedef*foc2,FOCmotor_Typedef*foc3)
//    //{
//    //    foc1->zero_electric_angle=electricalAngle(foc1->DIR,foc1->PP,foc1->zero_electric_angle,i2c_mt6701_get_angle2());
//    //    foc2->zero_electric_angle=electricalAngle(foc2->DIR,foc2->PP,foc2->zero_electric_angle,i2c_mt6701_get_angle1());
//    //    foc3->zero_electric_angle= 5.5;
//    //}
//    void Get_zeroangle(FOCmotor_Typedef*foc1)
//    {
//    foc1->zero_electric_angle=electricalAngle(foc1->DIR,foc1->PP,foc1->zero_electric_angle,MT6701_RecvData(My_I2C_SDA_GPIO_Port, My_I2C_SDA_Pin, My_I2C_SCL_GPIO_Port, My_I2C_SCL_Pin, 0));
//    }

//    void Get_ALL_zeroangleTEST(FOCmotor_Typedef*foc1,FOCmotor_Typedef*foc2,FOCmotor_Typedef*foc3)
//    {
//    foc1->zero_electric_angle=5.5;
//    foc2->zero_electric_angle=5.5;
//    foc3->zero_electric_angle= 5.5;
//    }

//    //获取所有角度
//    //void Get_ALL_angle(FOCmotor_Typedef*foc1,FOCmotor_Typedef*foc2,FOCmotor_Typedef*foc3)
//    //{
//    //    foc1->angle_el=electricalAngle(foc1->DIR,foc1->PP,foc1->zero_electric_angle,i2c_mt6701_get_angle2());
//    //    foc2->angle_el=electricalAngle(foc2->DIR,foc2->PP,foc2->zero_electric_angle,i2c_mt6701_get_angle1());
//    //    foc3->angle_el= 0;
//    //}

//    //void Get_angle(FOCmotor_Typedef*foc1)
//    //{
//    //    foc1->angle_re=i2c_mt6701_get_angle2();
//    //    foc1->angle_el=electricalAngle(foc1->DIR,foc1->PP,foc1->zero_electric_angle,foc1->angle_re);
//    //}

//    void Get_angle(FOCmotor_Typedef*foc1)
//    {
//    foc1->angle_re= MT6701_RecvData(My_I2C_SDA_GPIO_Port, My_I2C_SDA_Pin, My_I2C_SCL_GPIO_Port, My_I2C_SCL_Pin, 0);
//    foc1->angle_el=electricalAngle(foc1->DIR,foc1->PP,foc1->zero_electric_angle,foc1->angle_re);
//    }


//    // 设置PWM到控制器输出
//    float  dc_a,dc_b,dc_c;

//    void Motor_setPwm(float Ua,float Ub,float Uc,char ch) {
//    // 限制上限
//    Ua = func_limit_ab(Ua, 0.0f, voltage_power_supply);
//    Ub = func_limit_ab(Ub, 0.0f, voltage_power_supply);
//    Uc = func_limit_ab(Uc, 0.0f, voltage_power_supply);
//    // 计算占空比
//    // 限制占空比从0到1
//    dc_a = func_limit_ab(Ua / voltage_power_supply, 0.0f, 1.0f);
//    dc_b = func_limit_ab(Ub / voltage_power_supply, 0.0f, 1.0f);
//    dc_c = func_limit_ab(Uc / voltage_power_supply, 0.0f, 1.0f);
//    if (ch==1){M1_HAL_SETPWM(dc_a);}
//    //  else if (ch==2){M2_HAL_SETPWM(dc_a,dc_b,dc_c);}
//    //  else if (ch==3){M3_HAL_SETPWM(dc_a,dc_b,dc_c);}
//    //写入PWM到PWM 0 1 2 通道
//    }


//    //电机力矩孔控制
//    void Motor_setTorque(FOCmotor_Typedef*foc,float Uq1)
//    {
//    foc->Uq=Uq1;

//    if (foc->Uq < 0)foc->angle_el += _PI;

//    foc->Uq = fabs(foc->Uq);
//    foc->angle_el = normalizeAngle(foc->angle_el + _PI_2);
//    foc->sector = floor(foc->angle_el / _PI_3) + 1;
//    // calculate the duty cycles
//    foc->T1 = _SQRT3 * sin(foc->sector * _PI_3 - foc->angle_el) * foc->Uq / voltage_power_supply;
//    foc->T2 = _SQRT3 * sin(foc->angle_el - (foc->sector - 1.0) * _PI_3) * foc->Uq / voltage_power_supply;
//    foc->T0 = 1 - foc->T1 - foc->T2;

//    switch (foc->sector)
//    {
//    case 1:
//    foc->Ta = foc->T1 + foc->T2 + foc->T0 / 2;
//    foc->Tb = foc->T2 + foc->T0 / 2;
//    foc->Tc = foc->T0 / 2;
//    break;
//    case 2:
//    foc->Ta = foc->T1 + foc->T0 / 2;
//    foc->Tb = foc->T1 + foc->T2 + foc->T0 / 2;
//    foc->Tc = foc->T0 / 2;
//    break;
//    case 3:
//    foc->Ta = foc->T0 / 2;
//    foc->Tb = foc->T1 + foc->T2 + foc->T0 / 2;
//    foc->Tc = foc->T2 + foc->T0 / 2;
//    break;
//    case 4:
//    foc->Ta = foc->T0 / 2;
//    foc->Tb = foc->T1 + foc->T0 / 2;
//    foc->Tc = foc->T1 + foc->T2 + foc->T0 / 2;
//    break;
//    case 5:
//    foc->Ta = foc->T2 + foc->T0 / 2;
//    foc->Tb = foc->T0 / 2;
//    foc->Tc = foc->T1 + foc->T2 + foc->T0 / 2;
//    break;
//    case 6:
//    foc->Ta = foc->T1 + foc->T2 + foc->T0 / 2;
//    foc->Tb = foc->T0 / 2;
//    foc->Tc = foc->T1 + foc->T0 / 2;
//    break;
//    default:
//    foc->Ta = 0;
//    foc->Tb = 0;
//    foc->Tc = 0;
//    }

//    foc->Ua = foc->Ta * voltage_power_supply;
//    foc->Ub = foc->Tb * voltage_power_supply;
//    foc->Uc = foc->Tc * voltage_power_supply;
//    Motor_setPwm(foc->Ua,foc->Ub,foc->Uc,foc->ch);
//    }


//    //电机0电位初始化
//    void Motor_alignSensor(FOCmotor_Typedef*foc1) 
//    {
//    foc1->angle_el=_3PI_2;
//    Motor_setTorque(foc1, 4);  //起劲
//    HAL_Delay(1000);
//    //  Get_ALL_zeroangle(foc1);
//    Motor_setTorque(foc1, 0);  //起劲
//    }


//    void testdata(FOCmotor_Typedef*foc1,FOCmotor_Typedef*foc2,FOCmotor_Typedef*foc3,float data1)
//    {
//    printf("%f,%f,%f,%f,%f,%f\n",foc1->angle_re,foc1->Ua,foc1->Ub,foc1->Uc,foc1->zero_electric_angle,data1 );
//    }





