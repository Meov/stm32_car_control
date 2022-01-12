/**
  ******************************************************************************
  * @file main.c
  * @brief STM32F103C8T6 CANͨѶ
  * @author  �������ܿƼ�
  * @version  V2.2.0
  * @date     
  ******************************************************************************
  * @attention
  * 1.������ÿ1�뷢��һ��1֡8�ֽ�����
  * 2.�������ϲ��԰���PA0����ʱ ��������һ֡buf��������
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
u16 t=0;
u8 i=0;
u8 cnt=0;
u8 canbuf[8];
u8 res;


int init(void){
	int ret = 0;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);
 	LED_Init();	
	KEY_Init();//IO��ʼ��
	//EXTIX_Init();		 	//�ⲿ�жϳ�ʼ��
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);   
	
	return ERR_SUUCEED;
}

int main(void)
{	
	vu8 key=0;
	init();
	
	
 	while(1)
	{
		//��K0 ����CAN֡
		key=KEY_Scan(0);	//�õ���ֵ
		if(key)
			{			
				switch(key)
					{				 		
						case KEY0_PRES:	
						{
							LED0=!LED0;				
								for(i=0;i<8;i++)
								{
									canbuf[i]=cnt+i;//��䷢�ͻ�����			
								}	
									res=Can_Send_Msg(canbuf,8);//����8���ֽ� 
									break;
						}
					}
			 }else {
				 delay_ms(10); 

			 }
//����CAN֡
			
		key=Can_Receive_Msg(canbuf);
		if(key)//���յ�������
		{	
			printf("recieved something\n");
		}
			
		t++; 
		delay_ms(10);
		if(t==80)
		{
			for(i=0;i<8;i++)
			{
				canbuf[i]=cnt+i;//��䷢�ͻ�����			
			}	
			res=Can_Send_Msg(canbuf,8);//����8���ֽ� 
			LED0=!LED0;//��ʾϵͳ��������	
			t=0;
		}
	}		
	
 }

