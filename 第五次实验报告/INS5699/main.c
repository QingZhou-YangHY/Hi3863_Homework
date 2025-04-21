#include "pinctrl.h"
#include "common_def.h"
#include "soc_osal.h"
#include "i2c.h"
#include "osal_debug.h"
#include "app_init.h"
#include "timer.h"
#include "INS5699.h"

#define CONFIG_I2C_SCL_MASTER_PIN 15
#define CONFIG_I2C_SDA_MASTER_PIN 16
#define CONFIG_I2C_MASTER_PIN_MODE 2
#define I2C_MASTER_ADDR 0x0
#define I2C_SLAVE1_ADDR 0x38
#define I2C_SET_BANDRATE 400000
#define I2C_TASK_STACK_SIZE 0x1000
#define I2C_TASK_PRIO 17


void OledTask(void)
{
    uapi_pin_set_mode(CONFIG_I2C_SCL_MASTER_PIN, CONFIG_I2C_MASTER_PIN_MODE);
    uapi_pin_set_mode(CONFIG_I2C_SDA_MASTER_PIN, CONFIG_I2C_MASTER_PIN_MODE);
    uint32_t baudrate = 100000;
    uint32_t hscode = 0x00;
    errcode_t ret = uapi_i2c_master_init(1, baudrate, hscode);
    if (ret != 0) {
        printf("i2c init failed, ret = %0x\r\n", ret);
    }
    
    //初始化INS5699
    ins5699s_init();
    //设置初始时间 设置的是2025年4月21日星期1 8小时0分0秒
    ins5699s_time initial_time = {0};
    initial_time.sec = 0;
    initial_time.min = 0;
    initial_time.hour = 8;
    initial_time.week = 1;
    initial_time.day = 21;
    initial_time.month = 4;
    initial_time.year = 25;
    ins5699s_SetTime(initial_time);
    ins5699s_time current_time = ins5699s_GetTime();
    int real_current_time_year = current_time.year + 2000;
    while(1){
        osal_msleep(1000);
        osal_printk("现在是%4d年%2d月%2d日,星期%d,%02d:%02d:%02d\r\n",
        real_current_time_year,current_time.month,current_time.day,
        current_time.week,current_time.hour,current_time.min,
        current_time.sec);
        //更新时间
        current_time.sec ++;
        if(current_time.sec >= 60){
        current_time.sec = 0;
        current_time.min ++;
            if(current_time.min >= 60){
                current_time.min = 0;
                current_time.hour ++;
                if(current_time.hour >= 24){
                    current_time.hour = 0;
                    current_time.day ++;
                    current_time.week ++;
                    if(current_time.day >=31)//这里只写了设置的月份检验(4月份)
                    {
                        current_time.month ++;
                        current_time.day = 1;
                    }
                    if(current_time.week >= 8)
                    {
                        current_time.week = 1;
                    }
                    if(current_time.month >= 13)
                    {
                        current_time.year ++;
                        current_time.month = 1;
                    }
            }
        }
    }
    }
    
    
}

void oled_entry(void)
{
    uint32_t ret;
    osal_task *taskid;
    // 创建任务调度
    osal_kthread_lock();
    // 创建任务1
    taskid = osal_kthread_create((osal_kthread_handler)OledTask, NULL, "OledTask", I2C_TASK_STACK_SIZE);
    ret = osal_kthread_set_priority(taskid, I2C_TASK_PRIO);
    if (ret != OSAL_SUCCESS) {
        printf("create task1 failed .\n");
    }
    osal_kthread_unlock();
}

app_run(oled_entry);