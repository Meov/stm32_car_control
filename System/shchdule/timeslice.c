#include "timeslice.h"
#include "delay.h"
#include "misc.h"
#include "stm32f10x_tim.h"
#include "exti.h"

static LIST_HEAD(timeslice_task_list);

void timeslice_exec()
{
    ListObj* node;
    TimesilceTaskObj* task;
    list_for_each(node, &timeslice_task_list)
    {
           
        task = list_entry(node, TimesilceTaskObj, timeslice_task_list);
        if (task->is_run == TASK_RUN)
        {
            task->task_hdl();
            task->is_run = TASK_STOP;
        }
    }
}

void timeslice_tick()
{
    ListObj* node;
    TimesilceTaskObj* task;

    list_for_each(node, &timeslice_task_list)
    {
        task = list_entry(node, TimesilceTaskObj, timeslice_task_list);
        if (task->timer != 0)
        {
            task->timer--;
            if (task->timer == 0)
            {
                task->is_run = TASK_RUN;
                task->timer = task->timeslice_len;
            }
        }
    }
}

unsigned int timeslice_get_task_num()
{
    return list_len(&timeslice_task_list);
}

void timeslice_task_init(TimesilceTaskObj* obj, void (*task_hdl)(void), unsigned int id, unsigned int timeslice_len)
{
    obj->id = id;
    obj->is_run = TASK_STOP;
    obj->task_hdl = task_hdl;
    obj->timer = timeslice_len;
    obj->timeslice_len = timeslice_len;
}

void timeslice_task_add(TimesilceTaskObj* obj)
{
    list_insert_before(&timeslice_task_list, &obj->timeslice_task_list);
}

void timeslice_task_del(TimesilceTaskObj* obj)
{
    if (timeslice_task_isexist(obj))
        list_remove(&obj->timeslice_task_list);
    else
        return;
}


unsigned char timeslice_task_isexist(TimesilceTaskObj* obj)
{
    unsigned char isexist = 0;
    ListObj* node;
    TimesilceTaskObj* task;

    list_for_each(node, &timeslice_task_list)
    {
        task = list_entry(node, TimesilceTaskObj, timeslice_task_list);
        if (obj->id == task->id)
            isexist = 1;
    }

    return isexist;
}

unsigned int timeslice_get_task_timeslice_len(TimesilceTaskObj* obj)
{
    return obj->timeslice_len;
}

//使用TIMER2 进行时间片轮转调度
static int timer_init(void){
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);  //Timer2 时钟使能
	TIM_DeInit(TIM2);                              //复位TIM2定时器      
	TIM_TimeBaseStructure.TIM_Period=100;          //定时器周期
	TIM_TimeBaseStructure.TIM_Prescaler=36000-1;     //预分频数
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //TIM2时钟分频,为1表示不分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//定时器计数为向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);     
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);           //清除定时器2的溢出标志位
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);      //使能定时器2溢出中断
	TIM_Cmd(TIM2, ENABLE);                       //定时器2使能
}


void TIM2_IRQHandler(void){
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) //判断中断溢出标志为是否为1
	{
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); //清除溢出中断标志位
           
		//时间片刷新
		timeslice_tick();

     }
}

static void NVIC_Config(void){                                       //中断控制器的配置{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);           //优先组设置
	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn ;           //TIM2中断选通
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //使能中断控制
	NVIC_Init(&NVIC_InitStructure);

}

void  timeslice_execing(void)
{
    while (1)
    {
        timeslice_exec();
    }
}


void timeslice_tick_init(void){
	timer_init();
	NVIC_Config();
}



