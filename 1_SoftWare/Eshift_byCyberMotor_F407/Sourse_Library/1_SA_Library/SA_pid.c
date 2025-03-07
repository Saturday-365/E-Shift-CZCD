#include "SA_karman.h"
#include "SA_pid.h"
#include "SA_motor.h"

#include "math.h"
//-------------------------------------------------------------------------------------------------------------------
//  @brief 
//  @name
//  @return
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
//void SpeedPID_Init(SpeedPID_Typedef *pid)   //速度环结构体参数定义
//{
//	pid->SetSpeed=0;            //定义设定值
//	pid->ActualSpeed=0;        //定义实际值
//	pid->err=0;                //定义偏差值
//	pid->err_inte=0;            //定义偏差积分值
//	pid->err_last=0;			//定义上一次偏差值
//	pid->incrementSpeed=0;
//    pid->Kp=0;
//    pid->Ki=0;
//    pid->Kd=0; 
//}
//void AnglePD_Init(AnglePD_Typedef *pd)   //速度环结构体参数定义
//{
//	pd->SetAngle=0;                //定义设定值
//	pd->ActualAngle=0;            //定义实际值
//    pd-> err=0;                    //定义偏差值
//    pd-> err_inte=0;                //定义偏差积分值
//	pd-> err_last=0;                //定义上一次偏差值
//	pd-> Kp=-0.015;    
//	pd-> Kd=0.005;         //定义比例、积分、微分系数
//	pd-> incrementTorque=0;    
//}
////-------------------------------------------------------------------------------------------------------------------
////  @brief 
////  @name  //方向环pid
////  @return
////  Sample usage:
////-------------------------------------------------------------------------------------------------------------------
//float Angle_loop_PD(AnglePD_Typedef *pd,FOCmotor_Typedef *foc,float setAngle)
//{
////    pd->ActualAngle = foc->angle_re ;
//     pd->ActualAngle = foc->angle_re * 180 / PI;
//   pd->err = setAngle-pd->ActualAngle;
//    
//    pd->incrementTorque = pd -> Kp * pd->err + pd -> Kd*(pd->err-pd->err_last);
//    pd->err_last = pd->err;
//    func_limit_ab(pd->incrementTorque,-3,3);
//    return pd->incrementTorque;
//}
////-------------------------------------------------------------------------------------------------------------------
////  @brief 
////  @name  //速度环pid
////  @return
////  Sample usage:
////-------------------------------------------------------------------------------------------------------------------
//float Speed_loop_PD(AnglePD_Typedef *pd,FOCmotor_Typedef *foc,float setAngle)
//{
////    pd->ActualAngle = foc->angle_re ;
//   pd->ActualAngle = foc->angle_re * 180 / PI;
//   pd->err = setAngle-pd->ActualAngle;
//    
//    pd->incrementTorque = pd -> Kp * pd->err + pd -> Kd*(pd->err-pd->err_last);
//    pd->err_last = pd->err;
//    func_limit_ab(pd->incrementTorque,-3,3);
//    return pd->incrementTorque;
//}

////-------------------------------------------------------------------------------------------------------------------
////  @brief 
////  @name
////  @return
////  Sample usage:
////-------------------------------------------------------------------------------------------------------------------

////使用的模糊PID程序，有几个特殊步骤，输入的参数E为err，EC为err的微分，即这次的err减去上次的err
////这是模糊KP的程序，输入的参数为E（误差err）和EC（err的插值，及微分，err-last_err），返回的参数为动态的KP，然后带入普通的PID计算程序使用

////240627 01      
//    int rule_p[7][7]=
//    {
//        { 0 , 0 , 1 , 2 , 4 , 1 , 0},//-120
//        { 0 , 0 , 1 , 2 , 4 , 1 , 0},//-80
//        { 0 , 1 , 5 , 3 , 5 , 3 , 1},//-30
//        { 0 , 5 , 6 , 2 , 6 , 5 , 0},//0
//        { 1 , 3 , 5 , 3 , 5 , 1 , 0},// 30
//        { 0 , 1 , 4 , 2 , 1 , 0 , 0},//80
//        { 0 , 1 , 4 , 2 , 1 , 0 , 0},//120
//    };//模糊规则表 P
//    float EFF[7]  = {-256,-145,-65,0,65,145,256};   //非线性项
//   
//	float DFF[7]  = {-120,-81,-20, 0 , 20, 81, 120};  //非线性项  次项到达到角速度太大减弱p的目的
////   				  0   1   2   3   4   5   6 
//	float UpFF[10] = {0.71,0.74,0.77,0.80,0.83,0.86,0.89,0.92,0.95,0.98}; 	// 线性项  每项差距为0.03
//	//                 0    1    2    3    4    5    6     7    8    9
//	//float UpFF[7] = {0.69,0.73,0.77,0.81,0.85,0.89,0.93}; 	// 线性项  每项差距为0.04

//	float UdFF[4] = {2.83,2.76,2.68,2.60};     // d值减小线性项 每项差距0.05
//	//                 0    1    2    3         
////	float UdFF[13] = {2.0,3.00,2.95,2.90,2.85,2.80,2.75,2.70,2.65,2.60,2.55,2.50,2.45};     // d值减小线性项 每项差距0.05
//	
////	int rule_d[7] = { 3 , 2 , 1 , 0 , 1 , 2 , 3};//模糊规则表 D
//	int rule_d[7] = { 0,0,0,0,0,0,0};//模糊规则表 D

//    

//	float U=0;  /*偏差,偏差微分以及输出值的精确量*/
//    
//	float PF[2]={0,0},DF[2]={0,0},UF[4]={0,0};
//    
//	/*偏差,偏差微分以及输出值的隶属度*/
//    int Pn=0,Dn=0,Un[4]={0};
//    float t1=0,t2=0,t3=0,t4=0,temp1=0,temp2=0,ki=1;

//float KP_Fuzzy(float E,float EC)
//{
//    /*隶属度的确定*/
//    /*根据PD的指定语言值获得有效隶属度*/
//    if(E>EFF[0] && E<EFF[6])  //在EFF[7]={-110,-70,-30,0,30,70,110}// 在有效的err内
//    {						//假设此次的e为150
//        if(E<=EFF[1])      {Pn=-2;PF[0]=(EFF[1]-E)/(EFF[1]-EFF[0]);}      //偏差输出值的精确   pf 0=-150-e / 150-256       
//        else if(E<=EFF[2]) {Pn=-1;PF[0]=(EFF[2]-E)/(EFF[2]-EFF[1]);}       //偏差输出值的精确   pf 0=-60-e / -60+150   
//        else if(E<=EFF[3]) {Pn=0;PF[0]=(EFF[3]-E)/(EFF[3]-EFF[2]);} 		//偏差输出值的精确   pf 0=0-e / 0+60  
//        else if(E<=EFF[4]) {Pn=1;PF[0]=(EFF[4]-E)/(EFF[4]-EFF[3]);}
//        else if(E<=EFF[5]) {Pn=2;PF[0]=(EFF[5]-E)/(EFF[5]-EFF[4]);}    //Pn=2 PF0=  0
//        else if(E<=EFF[6]) {Pn=3;PF[0]=(EFF[6]-E)/(EFF[6]-EFF[5]);}
//    }
//    else if(E<=EFF[0]){Pn=-2; PF[0]=1;}
//    else if(E>=EFF[6]){Pn=3;  PF[0]=0;}
//    PF[1]=1-PF[0];	//pf两个值加在一起等于1                  
// 
//    //判断D的隶属度
//    if(EC>DFF[0]&&EC<DFF[6])
//    {
//        if(EC<=DFF[1])      {Dn=-2;DF[0]=(DFF[1]-EC)/(DFF[1]-DFF[0]);}
//        else if(EC<=DFF[2]) {Dn=-1;DF[0]=(DFF[2]-EC)/(DFF[2]-DFF[1]);}
//        else if(EC<=DFF[3]) {Dn=0;DF[0]=(DFF[3]-EC)/(DFF[3]-DFF[2]);}
//        else if(EC<=DFF[4]) {Dn=1;DF[0]=(DFF[4]-EC)/(DFF[4]-DFF[3]);}
//        else if(EC<=DFF[5]) {Dn=2;DF[0]=(DFF[5]-EC)/(DFF[5]-DFF[4]);}
//        else if(EC<=DFF[6]) {Dn=3;DF[0]=(DFF[6]-EC)/(DFF[6]-DFF[5]);}
//    }
//    //不在给定的区间内
//    else if (EC<=DFF[0]){Dn=-2;DF[0]=1;}
//    else if(EC>=DFF[6]){Dn=3;DF[0]=0;}
//    DF[1]=1-DF[0];
// 
//    /*使用误差范围优化后的规则表rule[7][7]*/
//    /*输出值使用13个隶属函数,中心值由UFF[7]指定*/
//    /*一般都是四个规则有效*/
//    Un[0]=rule_p[Pn+2][Dn+2];      //pn属于-2-3    pn=2
//    Un[1]=rule_p[Pn+3][Dn+2];
//    Un[2]=rule_p[Pn+2][Dn+3];
//    Un[3]=rule_p[Pn+3][Dn+3];
// 
//    if(PF[0]<=DF[0]) UF[0]=PF[0];   //求小    Pf
//    else 			 UF[0]=DF[0];
//    if(PF[1]<=DF[0]) UF[1]=PF[1];
//    else 			 UF[1]=DF[0];
//    if(PF[0]<=DF[1]) UF[2]=PF[0];
//    else 			 UF[2]=DF[1];
//    if(PF[1]<=DF[1]) UF[3]=PF[1];
//    else 			 UF[3]=DF[1];
//    /*同隶属函数输出语言值求大*/
//    if(Un[0]==Un[1])
//    {if(UF[0]>UF[1]) UF[1]=0;
//     else 			 UF[0]=0;}
//    if(Un[0]==Un[2])
//    {if(UF[0]>UF[2]) UF[2]=0;
//     else 			 UF[0]=0;}
//    if(Un[0]==Un[3])
//    {if(UF[0]>UF[3]) UF[3]=0;
//     else 			 UF[0]=0;}
//    if(Un[1]==Un[2])
//    {if(UF[1]>UF[2]) UF[2]=0;
//     else 			 UF[1]=0;}
//    if(Un[1]==Un[3])
//    {if(UF[1]>UF[3]) UF[3]=0;
//     else 			 UF[1]=0;}
//    if(Un[2]==Un[3])
//    {if(UF[2]>UF[3]) UF[3]=0;
//     else 			 UF[2]=0;}
//    
//	t1=UF[0]*UpFF[Un[0]];
//    t2=UF[1]*UpFF[Un[1]];
//    t3=UF[2]*UpFF[Un[2]];
//    t4=UF[3]*UpFF[Un[3]];
//    
//	temp1=t1+t2+t3+t4;
//    
//	temp2=UF[0]+UF[1]+UF[2]+UF[3];//模糊量输出
//    
//	if(temp2!=0)
//        U=temp1/temp2;
//    else {
//        U=0;
//    }
////    temp1=PF[0]*UFF[Un[0]]+PF[1]*UFF[Un[1]]+PF[0]*UFF[Un[2]]+PF[1]*UFF[Un[3]]+DF[0]*UFF[Un[0]]+DF[0]*UFF[Un[1]]+DF[1]*UFF[Un[2]]+DF[0]*UFF[Un[3]];
////    U=temp1;
//    return U;
//}





////-------------------------------------------------------------------------------------------------------------------
////  @brief 
////  @name
////  @return
////  Sample usage:
////-------------------------------------------------------------------------------------------------------------------
////下面是模糊KD的程序，使用的为一阶的，输入的参数为EC（err-last_err），返回的为动态的KD
////int rule_d[7] = { 3 , 2 , 1 , 0 , 1 , 2 , 3};//模糊规则表 D
//    float out=0;
//    uint8 i=0;
//    float degree_left = 0,degree_right = 0;
//    uint8 degree_left_index = 0,degree_right_index = 0;
//	//float UdFF[7]={2.7,2.5,2.2,2.35,1.7,1.95,1.6};
////	float UdFF[7]={1.60,1.30,1.70,1.40,2.45,1.60,1.8};
//	//                    1        3         5    6 	
////	float UdFF[7]={2.7,2.7,2.2,2.55,1.7,2.00,1.5};
//float Kd_Fuzzy(float ECc)
//{ 
//    if(ECc<DFF[0])
//    {
//        degree_left = 1;
//        degree_right = 0;
//        degree_left_index = 0;
//    }
//    else if (ECc>DFF[6]) {
//        degree_left = 1;
//        degree_right = 0;
//        degree_left_index = 6;
//    }
//    else {
//        for(i=0;i<6;i++)
//        {
//            if(ECc>=DFF[i]&&ECc<DFF[i+1])
//            {
//                degree_left = (float)(DFF[i+1] - ECc)/(DFF[i+1] - DFF[i]);
//                degree_right = 1 - degree_left;
//                degree_left_index = i;
//                degree_right_index = i+1;
//                break;
//            }
//        }
//    }
// 
//    out = UdFF[rule_d[degree_left_index]]*degree_left+UdFF[rule_d[degree_right_index]]*degree_right;
// 
//    return out;
//}

















