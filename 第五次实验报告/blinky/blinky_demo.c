// #include "pinctrl.h"
// #include "watchdog.h"
// #include "tcxo.h"
// #include "gpio.h"
// #include "soc_osal.h"
// #include "app_init.h"
// #include "watchdog.h"
// #include "ws2812b.h"
// #define BLINKY_DURATION_MS        500
// #define WS2812B_LED_0             5
// #define WS2812B_LED_M0DE          4
// #define BLINKY_TASK_PRIO          24
// #define BLINKY_TASK_STACK_SIZE    0x1000
// uint8_t WS2812B_BRIGHTNESS = 255;
// uint32_t preg_placeholder = 0;
// static __inline__ void one_num(void){
//     uapi_reg_setbit(0x44028030, 5);
//     for (int i = 0; i < 20; i++) {
//         uapi_reg_read32(0x44028030, preg_placeholder);
//     }
//     uapi_reg_setbit(0x44028034, 5);
//     for (int i = 0; i < 4; i++) {
//         uapi_reg_read32(0x44028030, preg_placeholder);
//     }
// }

// static __inline__ void zero_num(void){
//     uapi_reg_setbit(0x44028030, 5);
//     for (int i = 0; i < 4; i++) {
//         uapi_reg_read32(0x44028030, preg_placeholder);
//     }
//     uapi_reg_setbit(0x44028034, 5);
//     for (int i = 0; i < 18; i++) {
//         uapi_reg_read32(0x44028030, preg_placeholder);
//     }
// }


// void rgb_display(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness){
//     red = ((uint32_t)red) * brightness / 255;
//     green = ((uint32_t)green) * brightness / 255;
//     blue = ((uint32_t)blue) * brightness / 255;
//     for (int i = 0; i < 8; i++) {
//         if(red & 0x80) one_num();
//         else zero_num();
//         red <<= 1;
//     }
//     for (int i = 0; i < 8; i++) {
//         if(green & 0x80) one_num();
//         else zero_num();
//         green <<= 1;
//     }
//     for (int i = 0; i < 8; i++) {
//         if(blue & 0x80) one_num();
//         else zero_num();
//         blue <<= 1;
//     }
// }


// // 测试函数
// static void *tricolored_task(const char *arg) {
//     UNUSED(arg);
//     uapi_pin_set_mode(WS2812B_LED_0, WS2812B_LED_M0DE);
//     uapi_gpio_set_dir(WS2812B_LED_0, GPIO_DIRECTION_OUTPUT);
//     uapi_reg_setbit(0x44028034, 5);
//     uapi_tcxo_delay_us(500); // init
//     while(1){
//     for(uint8_t j = 0; j < 6; j++){
//         rgb_display(255, 0, 0, WS2812B_BRIGHTNESS);
//     }
//     osal_msleep(1);
//     for(uint8_t j = 0; j < 6; j++){
//         rgb_display(255, 0, 0, WS2812B_BRIGHTNESS);
//     }
//     osal_msleep(1000);
//     for(uint8_t j = 0; j < 6; j++){
//         rgb_display(0,255, 0, WS2812B_BRIGHTNESS);
//     }
//     osal_msleep(1);
//     for(uint8_t j = 0; j < 6; j++){
//         rgb_display(0,255, 0, WS2812B_BRIGHTNESS);
//     }
//     osal_msleep(1000);
//     for(uint8_t j = 0; j < 6; j++){
//         rgb_display(0, 0, 255, WS2812B_BRIGHTNESS);
//     }
//     osal_msleep(1);
//     for(uint8_t j = 0; j < 6; j++){
//         rgb_display(0, 0,255, WS2812B_BRIGHTNESS);
//         }
//     osal_msleep(1000);
//     return NULL;
//     }
// }

// static void blinky_entry(void)
// {
//     osal_task *task_handle = NULL;
//     osal_kthread_lock();
//     task_handle = osal_kthread_create((osal_kthread_handler)tricolored_task, 0, "BlinkyTask", BLINKY_TASK_STACK_SIZE);
//     if (task_handle != NULL) {
//         osal_kthread_set_priority(task_handle, BLINKY_TASK_PRIO);
//         osal_kfree(task_handle);
//     }
//     osal_kthread_unlock();
// }

// /* Run the blinky_entry. */
// app_run(blinky_entry);



#include "pinctrl.h"
#include "common_def.h"
#include "soc_osal.h"
#include "i2c.h"
#include "osal_debug.h"
#include "app_init.h"
#include "timer.h"


#define CONFIG_I2C_SCL_MASTER_PIN 15
#define CONFIG_I2C_SDA_MASTER_PIN 16
#define CONFIG_I2C_MASTER_PIN_MODE 2
#define I2C_MASTER_ADDR 0x0
#define I2C_SLAVE1_ADDR 0x38
#define I2C_SET_BANDRATE 400000
#define I2C_TASK_STACK_SIZE 0x1000
#define I2C_TASK_PRIO 17

//开启相关的I2C功能
// void app_i2c_init_pin(void)
// {
//     const char *arg;
//     UNUSED(arg);
//     uapi_pin_set_mode(CONFIG_I2C_SCL_MASTER_PIN, CONFIG_I2C_MASTER_PIN_MODE);
//     uapi_pin_set_mode(CONFIG_I2C_SDA_MASTER_PIN, CONFIG_I2C_MASTER_PIN_MODE);
// }

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