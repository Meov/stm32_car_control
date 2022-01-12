#include "hear_beat.h"
#include "timeslice.h"
#include "led.h"
#include "delay.h"
#include <stdio.h>

enum LED_STATE{
	OFF = 1,
	ON = 0,
};


TimesilceTaskObj task_ONE, task_TWO, task_THREE;

static void idle(){
	printf("idle...\n");
	delay_ms(5000);
	printf("idle done...\n");
}


static void led_heart_beat_on(void){
	LED0 = ON;
	delay_ms(20);
	LED0 = OFF;
	delay_ms(200);
	LED0 = ON;
	delay_ms(20);
	printf("blink\n");
}
static void led_heart_beat_off(void){
	LED0 = OFF;
	printf("off\n");
}


// 初始化任务对象，并且将任务添加到时间片轮询调度中
static void hear_beat_init()
{
    timeslice_task_init(&task_ONE, led_heart_beat_on,  1, 40); 
	timeslice_task_init(&task_TWO, led_heart_beat_off, 2, 20);
	timeslice_task_init(&task_THREE, idle, 3, 30);
  
    timeslice_task_add(&task_ONE);
    timeslice_task_add(&task_TWO);
	timeslice_task_add(&task_THREE);


}

void hear_beat(){
	
	hear_beat_init();
	
}
