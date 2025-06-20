#include "SA_eulerangles.h"
#include "SA_karman.h"
#include "common.h"
#include "math.h"

//#define Kp 30.0f                        // 比例增益控制加速度计/磁力计的收敛速率proportional gain governs rate of convergence to accelerometer/magnetometer
//#define Ki 0.3f                      // 积分增益控制陀螺仪偏置的收敛速率integral gain governs rate of convergence of gyroscope biases
//#define halfT 0.005f                   // half the sample period 采样周期的一半
//#define Gyro_Gr		0.0010653f		//角速度变成弧度 此参数对应陀螺 2000度每秒
//#define Gyro_G 		0.0610351f		//角速度变成度   此参数对应陀螺 2000度每秒	
//#define FILTER_NUM 	20




//float 	AngleOffset_Rol=0,AngleOffset_Pit=0,AngleOffset_Yaw=0;
//float q0 = 1, q1 = 0, q2 = 0, q3 = 0;  // quaternion elements representing the estimated orientation
//float exInt = 0, eyInt = 0, ezInt = 0; // scaled integral error
//int16 pullup_value;
//Kalman_Typedef IMUAcc_X_Kalman,IMUAcc_Y_Kalman,IMUAcc_Z_Kalman,IMUGyro_X_Kalman,IMUGyro_Y_Kalman,IMUGyro_Z_Kalman;
//FLOAT_XYZ 	Acc,Acc_out,Gyro,Gyro_out;	              //把陀螺仪的各通道读出的数据，转换成弧度制
//FLOAT_ANGLE Att_Angle;

//void Get_imuPose_Init()
//{
////	imu660ra_init();//陀螺仪init
//	Kalman_Init(&IMUAcc_X_Kalman,	0.0000001, 0.000001);
//	Kalman_Init(&IMUAcc_Y_Kalman,0.0000001, 0.000001);
//	Kalman_Init(&IMUAcc_Z_Kalman,	0.0000001, 0.000001);
//	Kalman_Init(&IMUGyro_X_Kalman,0.0000001, 0.000001);
//	Kalman_Init(&IMUGyro_Y_Kalman,	0.0000001, 0.000001);
//	Kalman_Init(&IMUGyro_Z_Kalman,0.0000001, 0.000001);
//	
//}
//void Get_imuPose()
//{
//	Get_IMUData(&Acc,&Gyro);
//	Karman_IMUData(&Acc,&Acc_out,&Gyro,&Gyro_out);
////	IMUupdate(&Gyro, &Acc, &Att_Angle);
//	IMUupdate(&Gyro_out, &Acc_out, &Att_Angle);

//}

//void Get_IMUData(FLOAT_XYZ *acc_data,FLOAT_XYZ *gyro_data)
//{
////		imu660ra_get_acc(); 		//获取加速度数据
////		imu660ra_get_gyro();    	//获取陀螺仪数据
////		gyro_data->X = imu660ra_gyro_transition(imu660ra_gyro_x)-GX_Zero; 		//将100次的陀螺仪数据累加最后处100
////		gyro_data->Y = imu660ra_gyro_transition(imu660ra_gyro_y)-GY_Zero; 		
////		gyro_data->Z = imu660ra_gyro_transition(imu660ra_gyro_z)-GZ_Zero; 		
////		acc_data->X  = imu660ra_acc_transition(imu660ra_acc_x); 		
////		acc_data->Y  = imu660ra_acc_transition(imu660ra_acc_y); 	
////		acc_data->Z  = imu660ra_acc_transition(imu660ra_acc_z); 
//}
//void Prepare_Data(FLOAT_XYZ *acc_in,FLOAT_XYZ *acc_out)//均值滤波
//{
//	static uint8 	filter_cnt=0;
//	static int16	ACC_X_BUF[FILTER_NUM],ACC_Y_BUF[FILTER_NUM],ACC_Z_BUF[FILTER_NUM];
//	int32 temp1=0,temp2=0,temp3=0;
//	uint8 i;

//	ACC_X_BUF[filter_cnt] = acc_in->X;
//	ACC_Y_BUF[filter_cnt] = acc_in->Y;
//	ACC_Z_BUF[filter_cnt] = acc_in->Z;
//	for(i=0;i<FILTER_NUM;i++)
//	{
//		temp1 += ACC_X_BUF[i];
//		temp2 += ACC_Y_BUF[i];
//		temp3 += ACC_Z_BUF[i];
//	}
//	acc_out->X = temp1 / FILTER_NUM;
//	acc_out->Y = temp2 / FILTER_NUM;
//	acc_out->Z = temp3 / FILTER_NUM;
//	filter_cnt++;
//	if(filter_cnt==FILTER_NUM)	filter_cnt=0;
//}

//void Karman_IMUData(FLOAT_XYZ *IMUdata_acc_in,FLOAT_XYZ *IMUdata_acc_out,FLOAT_XYZ *IMUdata_gyro_in,FLOAT_XYZ *IMUdata_gyro_out)
//{
//	IMUdata_acc_out->X=KalmanFilter(&IMUAcc_X_Kalman, IMUdata_acc_in->X);
//	IMUdata_acc_out->Y=KalmanFilter(&IMUAcc_Y_Kalman, IMUdata_acc_in->Y);
//	IMUdata_acc_out->Z=KalmanFilter(&IMUAcc_Z_Kalman, IMUdata_acc_in->Z);
//	IMUdata_gyro_out->X=KalmanFilter(&IMUGyro_X_Kalman, IMUdata_gyro_in->X);
//	IMUdata_gyro_out->Y=KalmanFilter(&IMUGyro_Y_Kalman, IMUdata_gyro_in->Y);
//	IMUdata_gyro_out->Z=KalmanFilter(&IMUGyro_Z_Kalman, IMUdata_gyro_in->Z);

//}
//void IMUupdate(FLOAT_XYZ *gyr, FLOAT_XYZ *acc, FLOAT_ANGLE *angle)
//{
//	float ax = acc->X,ay = acc->Y,az = acc->Z;
//	float gx = gyr->X,gy = gyr->Y,gz = gyr->Z;
//	float norm;
//	
//	float vx, vy, vz;
//	float ex, ey, ez;

//// 先把这些用得到的值算好
////	float q0q0 = q0*q0;
////	float q0q1 = q0*q1;
////	float q0q2 = q0*q2;
////	float q0q3 = q0*q3;
////	float q1q1 = q1*q1;
////	float q1q2 = q1*q2;
////	float q1q3 = q1*q3;
////	float q2q2 = q2*q2;
////	float q2q3 = q2*q3;
////	float q3q3 = q3*q3;

//	if(ax*ay*az==0)
//	return;

//	gx *= Gyro_Gr;
//	gy *= Gyro_Gr;
//	gz *= Gyro_Gr;

//	norm = sqrt(ax*ax + ay*ay + az*az);       //acc数据归一化
//	ax = ax /norm;
//	ay = ay / norm;
//	az = az / norm;
//	
////	if(norm>16500)
////	{Rc_C.ARMED=0;}
//	
//	// estimated direction of gravity and flux (v and w)      估计重力方向和流量/变迁
//	// 这是把四元数换算成《方向余弦矩阵》中的第三列的三个元素。
//	// 根据余弦矩阵和欧拉角的定义，地理坐标系的重力向量，转到机体坐标系，正好是这三个元素。
//	// 所以这里的vx\y\z，其实就是当前的欧拉角（即四元数）的机体坐标参照系上，换算出来的重力单位向量。      
//	vx = 2*(q1*q3 - q0*q2);												
//	vy = 2*(q0*q1 + q2*q3);
//	vz = q0*q0 - q1*q1 - q2*q2 + q3*q3 ;

//	//误差是磁场参考方向与传感器测量方向之间的叉积之和  error is sum of cross product between reference direction of fields and direction measured by sensors
//	ex = (ay*vz - az*vy) ;     	//向量外积在相减得到差分就是误差                   					
//	ey = (az*vx - ax*vz) ;
//	ez = (ax*vy - ay*vx) ;

//	exInt = exInt + ex * Ki;	//对误差进行积分
//	eyInt = eyInt + ey * Ki;
//	ezInt = ezInt + ez * Ki;

//	// adjusted gyroscope measurements
//	gx = gx + Kp*ex + exInt;	//将误差PI后补偿到陀螺仪，即补偿积分漂移
//	gy = gy + Kp*ey + eyInt;	
//	gz = gz + Kp*ez + ezInt;	//这里的gz由于没有观测者进行矫正会产生漂移，表现出来的就是积分自增或自减

//	// integrate quaternion rate and normalise	对四元数率进行积分和归一化					  
//	//四元素的微分方程，一阶毕卡求解法，更新四元数
//	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
//	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
//	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
//	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

//	// normalise quaternion 标准化四元数
//	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);

//	q0 = q0 / norm;
//	q1 = q1 / norm;
//	q2 = q2 / norm;
//	q3 = q3 / norm;

////	angle->rol = -asin(-2*q1q3+2*q0q2)* 57.3 - AngleOffset_Pit; // pitch
////	angle->pit = -atan2(2 * q2q3 + 2 * q0q1, -2 * q1q1 - 2 * q2q2 + 1)* 57.3 - AngleOffset_Rol; // roll
////	angle->yaw += -gyr->Z*Gyro_G*0.002f;

////	angle->pit = -asin(2*q1*q3-2*q0*q2)*57.3 - AngleOffset_Pit; // pitch   俯仰角
////	angle->rol = atan2(2*q2*q3+2*q0*q1,q0*q0-q1*q1-q2*q2+q3*q3)*57.3 - AngleOffset_Rol; // roll   翻滚角
////	angle->yaw = atan2(2*q1*q2+2*q0*q3,q0*q0+q1*q1-q2*q2-q3*q3)*57.3 - AngleOffset_Yaw;  //yaw   续航角

//	angle->pit = -asin(2*q1*q3-2*q0*q2)*57.3;// - AngleOffset_Pit; // pitch   俯仰角
//	angle->rol = atan2(2*q2*q3+2*q0*q1,-2*q1*q1-2*q2*q2+1)*57.3;// - AngleOffset_Rol; // roll   翻滚角
////	angle->rol = 0; //
//	angle->yaw = atan2(2*q1*q2+2*q0*q3,-2*q2*q2-2*q3*q3+1)*57.3;// - AngleOffset_Yaw;  //yaw   续航角
////	angle->yaw = 0;
//	//pullup_value=func_limit_ab(angle->pit,0,50);
//	pullup_value=(int16)angle->pit;

//}



//unsigned char ch0h[4],ch1h[4],ch2h[4],ch3h[4],ch4h[4],ch5h[4],ch6h[4],ch7h[4],ch8h[4],ch9h[4],ch10h[4];  
//unsigned char htail[4]={0x00, 0x00, 0x80, 0x7f};	//vofa输出帧尾


//void vofaIMUtest(FLOAT_XYZ *gyrtest, FLOAT_XYZ *acctest, FLOAT_ANGLE *angletest)
//{

//		Float_to_Byte(angletest->pit,ch0h);	
//		Float_to_Byte(angletest->rol,ch1h);	
//		Float_to_Byte(angletest->yaw,ch2h);		
//		Float_to_Byte(acctest->X,ch3h);	
//		Float_to_Byte(acctest->Y,ch4h);	
//		Float_to_Byte(acctest->Z,ch5h);	
//	
////		Float_to_Byte(GX_Zero,ch3h);	
////		Float_to_Byte(GY_Zero,ch4h);	
////		Float_to_Byte(GZ_Zero,ch5h);	
//		
//		Float_to_Byte(gyrtest->X,ch6h);	
//		Float_to_Byte(gyrtest->Y,ch7h);	
//		Float_to_Byte(gyrtest->Z,ch8h);//dutyDifferential
//		//Float_to_Byte(SpeedL_out,ch9h);		
//		wireless_uart_send_buff(ch0h,4);
//		wireless_uart_send_buff(ch1h,4);
//		wireless_uart_send_buff(ch2h,4);
//		wireless_uart_send_buff(ch3h,4);
//		wireless_uart_send_buff(ch4h,4);
//		wireless_uart_send_buff(ch5h,4);
//		wireless_uart_send_buff(ch6h,4);
//		wireless_uart_send_buff(ch7h,4);
//		wireless_uart_send_buff(ch8h,4);
////		wireless_uart_send_buff(ch9h,4);

//		wireless_uart_send_buff(htail,4);

//}
//void Std_angle_set()
//{
//	delay_ms(100);
//	std_angle=pullup_value;

//}
