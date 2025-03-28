#ifndef _SA_EulerAngles_H_
#define _SA_EulerAngles_H_
//#include "common.h"


//#define G			9.80665f		      	// m/s^2	
//#define RadtoDeg    57.324841f				//弧度到角度 (弧度 * 180/3.1415)
//#define DegtoRad    0.0174533f				//角度到弧度 (角度 * 3.1415/180)

////三轴浮点型
//typedef struct
//{
//	float X;
//	float Y;
//	float Z;
//}FLOAT_XYZ;

////姿态解算后的角度
//typedef struct
//{
//	float rol;//旋转角度
//	float pit;//俯仰
//	float yaw;//偏航
//}FLOAT_ANGLE;

//extern int16 pullup_value;
//extern FLOAT_XYZ 	Acc,Acc_out,Gyro_out,Gyro;	              //把陀螺仪的各通道读出的数据，转换成弧度制
//extern FLOAT_ANGLE Att_Angle;
//void Prepare_Data(FLOAT_XYZ *acc_in,FLOAT_XYZ *acc_out);
//void IMUupdate(FLOAT_XYZ *Gyr_rad,FLOAT_XYZ *Acc_filt,FLOAT_ANGLE *Att_Angle);
//void Get_imuPose_Init();
//void Get_imuPose();
//void Get_IMUData(FLOAT_XYZ *acc_data,FLOAT_XYZ *gyro_data);
//void Karman_IMUData(FLOAT_XYZ *IMUdata_acc_in,FLOAT_XYZ *IMUdata_acc_out,FLOAT_XYZ *IMUdata_gyro_in,FLOAT_XYZ *IMUdata_gyro_out);
//void vofaIMUtest(FLOAT_XYZ *gyrtest, FLOAT_XYZ *acctest, FLOAT_ANGLE *angletest);
//void Std_angle_set();

#endif



////#include "IMU.h"

////Prepare_Data(&Acc,&Acc_out);//Acc输入、Acc_out输出
////IMUupdate(&Gyro,&Acc_out,&Att_Angle);




