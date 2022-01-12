/**
  ******************************************************************************
  * @file main.c
  * @brief STM32F103C8T6 CAN通讯
  * @author  昌雯智能科技
  * @version  V2.2.0
  * @date     
  ******************************************************************************
  * @attention
  * 1.开发板每1秒发送一次1帧8字节数据
  * 2.开发板上测试按键PA0按下时 立即发送一帧buf填充的数据
  ******************************************************************************
  */ 
#include "timeslice.h"
#include "err_code.h"
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "exti.h"
#include "can.h" 
#include "hear_beat.h"
u16 t=0;
u8 i=0;
u8 cnt=0;
u8 canbuf[8];
u8 res;

//#ifdef TEST

#include "test.h"

//#endif




void timeslice_init(void){
	timeslice_tick_init();  //时间片轮询初始化
}

void system_running(void){
	timeslice_execing();    //时间片开始调度
}

int hardware_init(void){
	int ret = 0;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);
 	LED_Init();	
	KEY_Init();//IO初始化
	EXTIX_Init();		 	//外部中断初始化
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);   
	
	return ERR_SUUCEED;
}


int main(int argc, char* argv[]){
	
	hardware_init();
	timeslice_init();
	hear_beat();
	
	system_running(); //while 1
}


/*

int main(void)
{	
	vu8 key=0;
	init();
	test();
	
 	while(1)
	{
		//按K0 发送CAN帧
		key=KEY_Scan(0);	//得到键值
		if(key)
			{			
				switch(key)
					{				 		
						case KEY0_PRES:	
						{
							LED0=!LED0;				
								for(i=0;i<8;i++)
								{
									canbuf[i]=cnt+i;//填充发送缓冲区			
								}	
									res=Can_Send_Msg(canbuf,8);//发送8个字节 
									break;
						}
					}
			 }else {
				 delay_ms(10); 

			 }
//接收CAN帧
			
		key=Can_Receive_Msg(canbuf);
		if(key)//接收到有数据
		{	
			printf("recieved something\n");
		}
			
		t++; 
		delay_ms(10);
		if(t==80)
		{
			for(i=0;i<8;i++)
			{
				canbuf[i]=cnt+i;//填充发送缓冲区			
			}	
			res=Can_Send_Msg(canbuf,8);//发送8个字节 
			LED0=!LED0;//提示系统正在运行	
			t=0;
		}
	}		
	
 }

*/
