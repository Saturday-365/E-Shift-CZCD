#include "SA_Flash.h"
#include "stdio.h"
#include "headfile.h"
#include "main.h"
uint16_t Store_Data[STORE_COUNT];				//定义SRAM数组

/**
  * 函    数：参数存储模块初始化
  * 参    数：无
  * 返 回 值：无
  */
void Store_Init(void)
{
	/*判断是不是第一次使用*/
	
    if (FLASH_ReadHalfWord(STORE_START_ADDRESS) == 0xA5A5) 
        {
        HAL_GPIO_WritePin((GPIO_TypeDef *)LED1_GPIO_Port, (uint16_t)LED1_Pin, (GPIO_PinState)0); 	
        }
        
    if (FLASH_ReadHalfWord(STORE_START_ADDRESS) != 0xA5A5)	//读取第一个半字的标志位，if成立，则执行第一次使用的初始化
	{
		FLASH_ErasePage(STORE_START_ADDRESS,1);					//擦除指定页
		HAL_FLASH_Unlock();
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,STORE_START_ADDRESS, 0xA5A5);	//在第一个半字写入自己规定的标志位，用于判断是不是第一次使用
		HAL_GPIO_WritePin((GPIO_TypeDef *)LED1_GPIO_Port, (uint16_t)LED1_Pin, (GPIO_PinState)1); 

        for (uint16_t i = 1; i < STORE_COUNT; i ++)				//循环STORE_COUNT次，除了第一个标志位
		{
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,STORE_START_ADDRESS + i * 2, 0x0000);		//除了标志位的有效数据全部清0
		}
        HAL_FLASH_Lock();
	}
        /*上电时，将闪存数据加载回SRAM数组，实现SRAM数组的掉电不丢失*/
	for (uint16_t i = 0; i < STORE_COUNT; i ++)					//循环STORE_COUNT次，包括第一个标志位
	{
		Store_Data[i] = FLASH_ReadHalfWord(STORE_START_ADDRESS + i * 2);		//将闪存的数据加载回SRAM数组
	}
}
 //数组保存到闪存
void Store_Save(void)
 {
    FLASH_ErasePage(STORE_START_ADDRESS,1);             //擦除指定页
    HAL_FLASH_Unlock();
    for (uint16_t i = 0; i < STORE_COUNT; i ++)         //循环STORE_COUNT次，包括第一个标志位
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,STORE_START_ADDRESS + i * 2,Store_Data[i]);  //将SRAM数组的数据备份保存到闪存
    }
    HAL_FLASH_Lock();
 }
 
 void Store_Clear(void)
{
    for (uint16_t i = 1; i < STORE_COUNT; i ++)  //循环STORE_COUNT次，除了第一个标志位      
    {
        Store_Data[i] = 0x0000;    //SRAM数组有效数据清0                     
    }
    Store_Save();               //保存数据到闪存                         
}
/**
  * 函    数：FLASH擦除指定地址
  * 参    数：无
  * 返 回 值：无
  */
void FLASH_ErasePage(uint32_t ErasePageBaseAddr,uint32_t ErasePageNbPageCount)
    {
        HAL_FLASH_Unlock();
        FLASH_EraseInitTypeDef EraseInitStruct = {
            .TypeErase = FLASH_TYPEERASE_PAGES,   //页擦除    
            .PageAddress = ErasePageBaseAddr, //擦除地址               
            .NbPages = ErasePageNbPageCount     //擦除页数                         
            };
        uint32_t PageError = 0;
        __disable_irq();    //擦除前关闭中断                         
        if (HAL_FLASHEx_Erase(&EraseInitStruct,&PageError) == HAL_OK)
        {
            printf("擦除 成功\r\n");
        }
        __enable_irq();
        HAL_FLASH_Lock();    //加锁              
    }


/*从指定地址开始写入数据*/
//FLASH_Addr:起始地址
//FLASH_Data:写入数据指针(写入的数据为16位，至少都要16位)
//Size:写入数据长度
void FLASH_WriteData(uint32_t FLASH_Addr, uint16_t *FLASH_Data, uint16_t Size)
{
	/*内部FLASH测试*/
	//1.解锁FLASH
	HAL_FLASH_Unlock();
	
//	//2.擦除FALSH
//	//初始化FLASH_EraseInitTypeDef
//	FLASH_EraseInitTypeDef FLASH_Init;
//	FLASH_Init.TypeErase = FLASH_TYPEERASE_PAGES;
//	FLASH_Init.PageAddress = FLASH_Addr;
//	FLASH_Init.NbPages = 1;
//	
//	uint32_t PageError = 0;
//	
//	//3.调用擦除函数
//	HAL_FLASHEx_Erase(&FLASH_Init,&PageError);
	FLASH_ErasePage(FLASH_Addr,1);
	
    //4.对FLASH烧写
	uint16_t TempBuf = 0;
	for(uint16_t i=0;i<Size;i++)
	{
		TempBuf = *(FLASH_Data+i);
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,(FLASH_Addr + (i*2)),TempBuf);
	}
	
	//5.锁住FLASH
	HAL_FLASH_Lock();
}


//从指定地址读出数据
/**
  * 函    数：FLASH读取一个32位的字
  * 参    数：Address 要读取数据的字地址
  * 返 回 值：指定地址下的数据
  */
uint32_t FLASH_ReadWord(uint32_t Address)
{
	return *((__IO uint32_t *)(Address));	//使用指针访问指定地址下的数据并返回
}

/**
  * 函    数：FLASH读取一个16位的半字
  * 参    数：Address 要读取数据的半字地址
  * 返 回 值：指定地址下的数据
  */
uint16_t FLASH_ReadHalfWord(uint32_t Address)
{
	return *((__IO uint16_t *)(Address));	//使用指针访问指定地址下的数据并返回
}

/**
  * 函    数：FLASH读取一个8位的字节
  * 参    数：Address 要读取数据的字节地址
  * 返 回 值：指定地址下的数据
  */
uint8_t FLASH_ReadByte(uint32_t Address)
{
	return *((__IO uint8_t *)(Address));	//使用指针访问指定地址下的数据并返回
}


//从指定地址开始读出指定长度的数据
//ReadAddr:起始指针
//pBuffer:数据指针
//NumToRead:大小（至少半字(16位)数）
void FLASH_ReadData(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)
{
	for(uint16_t i=0;i<NumToRead;i++)
	{
		pBuffer[i] = FLASH_ReadHalfWord(ReadAddr);
		ReadAddr+=2;
	}
}
