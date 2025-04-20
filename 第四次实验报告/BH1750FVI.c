#include "pinctrl.h"
#include "common_def.h"
#include "soc_osal.h"
#include "gpio.h"
#include "hal_gpio.h"
#include "app_init.h"
#include "BH1750FVI.h"
#include "i2c.h"
#define I2C_MASTER_BUS_ID 1
uint16_t bh1750_buff_H = 0;
uint16_t bh1750_buff_L = 0;

void bh1750_SendCMD(uint8_t cmd){
    uint8_t buffer[] = {cmd};
    i2c_data_t data = {0};
    data.send_buf = buffer;
    data.send_len = sizeof(buffer);
    errcode_t ret = uapi_i2c_master_write(I2C_MASTER_BUS_ID, 
BH1750_ADDR >> 1, &data);
    if (ret != 0) {
        printf("BH1750:I2cWriteCMD(%02X) failed, %0X!\n", cmd, ret);
    }
}
void bh1750_ReadData(void){
    uint8_t buffer[2] = {0};
    i2c_data_t data;
    data.receive_len = sizeof(buffer);
    data.receive_buf = buffer;
    errcode_t ret = uapi_i2c_master_read(I2C_MASTER_BUS_ID, 
BH1750_ADDR >> 1, &data);
    if (ret != 0) {
        printf("BH1750:I2cRead(len:%d) failed, %0X!\n", data.receive_len, 
ret);
    }
    bh1750_buff_H = data.receive_buf[0];
    bh1750_buff_L = data.receive_buf[1];
}



uint16_t bh1750_GetLightIntensity(void){
    uint16_t data;
    bh1750_SendCMD(BH1750_POWER_ON); // 开启设备
    bh1750_SendCMD(MEASURE_MODE);
    osal_msleep(200);
    bh1750_ReadData();
    //中间部分需要用户完成 共5行
    data = (bh1750_buff_H << 8) | bh1750_buff_L; 
    return (data * BH1750_RES)/1.2;
}
void bh1750_init(void){
    bh1750_SendCMD(BH1750_POWER_ON); // 开启设备
    bh1750_SendCMD(BH1750_MODULE_RESET); // 复位设备
    //中间部分需要用户完成 2行
    osal_msleep(200);
    osal_printk("BH1750 Init SUCC!\r\n");
}
