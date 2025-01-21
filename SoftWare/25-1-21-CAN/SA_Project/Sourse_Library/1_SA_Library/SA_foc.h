#ifndef _SA_FOC_H
#define _SA_FOC_H

#define _SQRT3 1.73205080757f
#define _SQRT3_2 0.86602540378f
#define _1_SQRT3 0.57735026919f
#define _2_SQRT3 1.15470053838f

#define PI 3.14159265359f
#define _PI     3.14159265359f
#define _PI_2   1.57079632679f
#define _PI_3   1.04719755120f
#define _3PI_2  4.71238898038f

#define func_limit_ab(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

typedef struct
{
    float Ua,dc_a,Ub,dc_b,Uc,dc_c,Uq,angle_el,angle_re; 
    char ch;
    float zero_electric_angle;
    float T0, T1, T2, Ta, Tb, Tc;

    int PP,DIR,sector;
   

}FOCmotor_Typedef;

void EN_Motor(void);
void HAL_SETPWM_init(void);

void FOCType_Init(FOCmotor_Typedef*foc,int _PP, int _DIR,char _ch);
void FOCType_setfirst(FOCmotor_Typedef*foc); //结构体参数定义

float electricalAngle(int DIR,int PP,float zero_electric_angle,float anglein) ;

void Motor_setTorque(FOCmotor_Typedef*foc,float Uq1);

void Motor_alignSensor(FOCmotor_Typedef*foc1);

void Get_ALL_angle(FOCmotor_Typedef*foc1,FOCmotor_Typedef*foc2,FOCmotor_Typedef*foc3);
void Get_ALL_zeroangle(FOCmotor_Typedef*foc1,FOCmotor_Typedef*foc2,FOCmotor_Typedef*foc3);
void Get_ALL_zeroangleTEST(FOCmotor_Typedef*foc1,FOCmotor_Typedef*foc2,FOCmotor_Typedef*foc3);
void Get_zeroangle(FOCmotor_Typedef*foc1);
void Get_angle(FOCmotor_Typedef*foc1);

void M1_HAL_SETPWM(float a1,float b1,float c1);
void M2_HAL_SETPWM(float a2,float b2,float c2);
void M3_HAL_SETPWM(float a3,float b3,float c3);
float normalizeAngle(float angle) ;
void Motor_setPwm(float Ua,float Ub,float Uc,char ch) ;

void testdata(FOCmotor_Typedef*foc1,FOCmotor_Typedef*foc2,FOCmotor_Typedef*foc3,float data1);


#endif

// void M0_setTorque(float Uq, float angle_el) {
//   Uq = _constrain(Uq, -(voltage_power_supply) / 2, (voltage_power_supply) / 2);
//   // Serial.printf("M0%f\n",Uq);
//   float Ud = 0;
//   angle_el = _normalizeAngle(angle_el);
//   // 帕克逆变换
//   float Ualpha = -Uq * sin(angle_el);
//   float Ubeta = Uq * cos(angle_el);

//   // 克拉克逆变换
//   float Ua = Ualpha + voltage_power_supply / 2;
//   float Ub = (sqrt(3) * Ubeta - Ualpha) / 2 + voltage_power_supply / 2;
//   float Uc = (-Ualpha - sqrt(3) * Ubeta) / 2 + voltage_power_supply / 2;
//   M0_setPwm(Ua, Ub, Uc);
// }

// void M1_setTorque(float Uq, float angle_el) {
//   Uq = _constrain(Uq, -(voltage_power_supply) / 2, (voltage_power_supply) / 2);
//   // Serial.printf("M1%f\n",Uq);
//   float Ud = 0;
//   angle_el = _normalizeAngle(angle_el);
//   // 帕克逆变换
//   float Ualpha = -Uq * sin(angle_el);
//   float Ubeta = Uq * cos(angle_el);

//   // 克拉克逆变换
//   float Ua = Ualpha + voltage_power_supply / 2;
//   float Ub = (sqrt(3) * Ubeta - Ualpha) / 2 + voltage_power_supply / 2;
//   float Uc = (-Ualpha - sqrt(3) * Ubeta) / 2 + voltage_power_supply / 2;
//   M1_setPwm(Ua, Ub, Uc);
// }




