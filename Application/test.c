#include <stdio.h>
#include "test.h"
#include "timeslice.h"
#include "delay.h"
#include "stm32f10x_tim.h"
#include "exti.h"
#include "timeslice.h"


TimesilceTaskObj task_1, task_2, task_3, task_4, task_5; 

// 具体的任务函数
void task1_hdl()
{
    printf(">> task 1 is running ...\n");
}

void task2_hdl()
{
    printf(">> task 2 is running ...\n");
}

void task3_hdl()
{
    printf(">> task 3 is running ...\n");
}

void task4_hdl()
{
    printf(">> task 4 is running ...\n");
}

void task5_hdl()
{
    printf(">> task 5 is running ...\n");
}

// 初始化任务对象，并且将任务添加到时间片轮询调度中
void task_init()
{
    timeslice_task_init(&task_1, task1_hdl, 1, 1);
    timeslice_task_init(&task_2, task2_hdl, 2, 2);
    timeslice_task_init(&task_3, task3_hdl, 3, 3);
    timeslice_task_init(&task_4, task4_hdl, 4, 4);
    timeslice_task_init(&task_5, task5_hdl, 5, 5);

    timeslice_task_add(&task_1);
    timeslice_task_add(&task_2);
    timeslice_task_add(&task_3);
    timeslice_task_add(&task_4);
    timeslice_task_add(&task_5);

}

void test(){
	printf("test begin\n");
	timeslice_tick_init();
	task_init();
	
    printf(">> task num: %d\n", timeslice_get_task_num());
    printf(">> task len: %d\n", timeslice_get_task_timeslice_len(&task_3));

    timeslice_task_del(&task_2);
    printf(">> delet task 2\n");
    printf(">> task 2 is exist: %d\n", timeslice_task_isexist(&task_2));

    printf(">> task num: %d\n", timeslice_get_task_num());

    timeslice_task_del(&task_5);
    printf(">> delet task 5\n");

    printf(">> task num: %d\n", timeslice_get_task_num());

    printf(">> task 3 is exist: %d\n", timeslice_task_isexist(&task_3));
    timeslice_task_add(&task_2);
    printf(">> add task 2\n");
    printf(">> task 2 is exist: %d\n", timeslice_task_isexist(&task_2));

    timeslice_task_add(&task_5);
    printf(">> add task 5\n");

    printf(">> task num: %d\n", timeslice_get_task_num());

    printf("\n\n========timeslice running===========\n");

	
	timeslice_execing();  //时间片调度
}
