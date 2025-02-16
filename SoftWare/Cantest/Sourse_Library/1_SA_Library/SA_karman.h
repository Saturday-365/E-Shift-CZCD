#ifndef __TXZ_KARMAN_H__
#define __TXZ_KARMAN_H__

typedef struct
{
    /*不用动*/
    float LastP;//上次估算协方差
    float Now_P;//当前估算协方差
    float out;//卡尔曼滤波器输出
    float Kg;//卡尔曼增益
	float Q;
	float R;
}Kalman_Typedef;

void Kalman_Init(Kalman_Typedef *klm, const float klm_Q, const float klm_R);
float KalmanFilter(Kalman_Typedef *klm, float input);


typedef struct
{
    /*不用动*/
    float k;//低通滤波系数
	float neww;//新的值
	float oldd;//旧的值
}Lowfloat_Typedef;

float Lowfloat_Fitier(Lowfloat_Typedef *lowf,float newww,float olddd);
void Lowfloat_Init(Lowfloat_Typedef *lowf,float lowf_k);

#endif

