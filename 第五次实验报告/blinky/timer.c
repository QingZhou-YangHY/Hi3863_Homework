#include "pinctrl.h"
#include "common_def.h"
#include "soc_osal.h"
#include "timer.h"
#include "osal_debug.h"
#include "app_init.h"
#include "tcxo.h"
#include "chip_core_irq.h"

#define TIMER_TASK_STACK_SIZE 0x1000
#define TIMER_TASK_PRIO 17 // 任务的优先级，数值越小优先级越高
#define TIMER_INDEX 1
#define TIMER_PRIO 1
#define TIMER_DELAY_INT 5
#define TIMER1_DELAY_1S 1000000 // 延时1s

timer_handle_t timer_index= 0;
typedef struct timer_info {
    uint32_t start_time;
    uint32_t end_time;
    uint32_t delay_time;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} timer_info_t;

static timer_info_t g_timer_info= {0, 0, TIMER1_DELAY_1S, 0, 0, 0};

//定时器Timer触发的回调函数
void TimerCallback(uintptr_t data)
{
    uint32_t timer = (uint32_t)data;
    (void)timer;  // 添加一个空的使用语句，避免警告
    g_timer_info.end_time = uapi_tcxo_get_ms();
    osal_printk("This is TimerCallback real time = %dms\r\n",
                (g_timer_info.end_time - g_timer_info.start_time));
    //更新时间
    g_timer_info.second ++;
    if(g_timer_info.second >= 60){
        g_timer_info.second = 0;
        g_timer_info.minute ++;
        if(g_timer_info.minute >= 60){
            g_timer_info.minute = 0;
            g_timer_info.hour ++;
            if(g_timer_info.hour >= 60){
                g_timer_info.hour = 0;
            }
        }
    }

    //显示时间
    osal_printk("现在时间为：%2d : %2d : %2d\r\n",g_timer_info.hour,
    g_timer_info.minute,g_timer_info.second);

    //在回调函数里面重新启动定时器
    g_timer_info.start_time = g_timer_info.end_time;
    uapi_timer_start(timer_index,g_timer_info.delay_time,TimerCallback,0);
}

//Timer的主程序入口
static void *timer_task(const char *arg)
{
    unused(arg);
    uapi_timer_init();
    uapi_timer_adapter(TIMER_INDEX, TIMER_1_IRQN, TIMER_PRIO);
    // 创建定时器
    uapi_timer_create(TIMER_INDEX, &timer_index);
    g_timer_info.start_time = uapi_tcxo_get_ms();
    uapi_timer_start(timer_index, g_timer_info.delay_time, 
    TimerCallback, 0); // 0代表定时器参数，用于传递给定时器回调函数
    return NULL;
}

static void timer_entry(void)
{
    uint32_t ret;
    osal_task *taskid;
    // 创建任务调度
    osal_kthread_lock();
    // 创建任务
    taskid = osal_kthread_create((osal_kthread_handler)timer_task, NULL, "timer_task",
                                 TIMER_TASK_STACK_SIZE);
    ret = osal_kthread_set_priority(taskid, TIMER_TASK_PRIO);
    if (ret != OSAL_SUCCESS) {
        printf("create task failed .\n");
    }
}
app_run(timer_entry);