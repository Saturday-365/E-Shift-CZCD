
#include "ATK_UART.h"
#include "headfile.h"

atk_ms601m_attitude_data_t IMUDATA;
int cc;
char IMU_uart_data;
char IMUDataBuff[12];
int IMURxLine,rec;
uint8_t IMU_uart_data_byte[8];

//uint8_t IMU_data_ok=1;
uint8_t step=0;//状态变量初始化为0 在函数中必须为静态变量
uint8_t len,cmd,*data_ptr;
uint8_t IMU_uart_callback_BYTE(uint8_t uart_tran[10] )
{	   
        if( uart_tran[0]==0x55 && 
            uart_tran[1]==0x55 &&
            uart_tran[2]==0x01 &&
            uart_tran[3]==0x06    )
        {
            Get_IMUuartData_BYTE(&IMUDATA, (uint8_t *)uart_tran);
            return 1;
        }
        else return 0;
}


void IMU_uart_callback_BIT(uint8_t uart_tran )
{	   
    IMU_uart_data=uart_tran;
    switch(step)
	{
	    case 0://接收帧头1状态
	        if(IMU_uart_data== 0x55)
	        {
	            rec=0;step++;//memset(IMUDataBuff,0,sizeof(IMUDataBuff));  //清空缓存数组
                IMURxLine = 0;
	            IMUDataBuff[IMURxLine++] = IMU_uart_data;               
	        }break;
	    case 1://接收帧头2状态
	        if(IMU_uart_data== 0x55)
	        {step++;   IMUDataBuff[IMURxLine++] = IMU_uart_data;}
	        else
	        {step = 0;}
	        break;
	    case 2://接收数据ID
	        if(IMU_uart_data== 0x01)
	        {   step++;
	            IMUDataBuff[IMURxLine++] = IMU_uart_data;}
	        else
	        {step = 0;}
	        break;
	    case 3://接收数据长度字节状态
	        if(IMU_uart_data== 0x06)
	        {
	            step++;
	            IMUDataBuff[IMURxLine++] = IMU_uart_data;
	        }
	        else
	        {
	            step = 0;
	        }
	        break;
	    case 4://接收len字节数据状态
	        step++;
	         IMUDataBuff[IMURxLine++] = IMU_uart_data;
	        break;
	    case 5://接收crc16校验高8位字节
	        step++;
	         IMUDataBuff[IMURxLine++] = IMU_uart_data ;
	        break;
	    case 6://接收crc16校验低8位字节
	        step++;
	         IMUDataBuff[IMURxLine++] = IMU_uart_data ;
	        break;
	    case 7://接收帧尾
	        step++;
	         IMUDataBuff[IMURxLine++] = IMU_uart_data ;
	        break;
	    case 8://接收帧尾
	        step++;
	         IMUDataBuff[IMURxLine++] = IMU_uart_data ;
	        break;
	    case 9://接收帧尾
	        step=0;
	         IMUDataBuff[IMURxLine] = IMU_uart_data ;      
              Get_IMUuartData(&IMUDATA);//数据解析和参数赋值函数
              rec=1;     
            break;
	    
        default:step=0;break;//多余状态，正常情况下不可能出现
	}
                        
}

void Get_IMUuartData(atk_ms601m_attitude_data_t *attitude_dat )
{        
    attitude_dat->roll = (float)((int16_t)(IMUDataBuff[5] << 8) | IMUDataBuff[4]) / 32768 * 180+180;
    attitude_dat->pitch = (float)((int16_t)(IMUDataBuff[7] << 8) | IMUDataBuff[6]) / 32768 * 180+180;
    attitude_dat->yaw = (float)((int16_t)(IMUDataBuff[9] << 8) | IMUDataBuff[8]) / 32768 * 180+180;
}
void Get_IMUuartData_BYTE(atk_ms601m_attitude_data_t *attitude_dat,uint8_t uart_tran[9] )
{        
    attitude_dat->roll = (float)((int16_t)(uart_tran[5] << 8) | uart_tran[4]) / 32768 * 180 +180;
    attitude_dat->pitch = (float)((int16_t)(uart_tran[7] << 8) | uart_tran[6]) / 32768 * 180+180;
    attitude_dat->yaw = (float)((int16_t)(uart_tran[9] << 8) | uart_tran[8]) / 32768 * 180+180;
}

void IMUuartData(atk_ms601m_attitude_data_t *attitude_tdat )
{        
    if( Report_stage() ) printf("%f,%f,%f\r\n",attitude_tdat->roll,attitude_tdat->pitch,attitude_tdat->yaw);	
}

float Get_IMUData(atk_ms601m_attitude_data_t *attitude_tdat,unsigned char mod    )
{
if (mod == 1 ) return attitude_tdat->roll;
else if(mod == 2)   return attitude_tdat->pitch;
else return attitude_tdat->yaw;
}


