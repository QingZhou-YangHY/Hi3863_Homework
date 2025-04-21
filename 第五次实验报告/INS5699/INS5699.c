#include "pinctrl.h"
#include "common_def.h"
#include "soc_osal.h"
#include "gpio.h"
#include "hal_gpio.h"
#include "app_init.h"
#include "i2c.h"
#include "INS5699.h"
#define I2C_MASTER_BUS_ID 1
void ins5699s_SendREG(uint8_t reg, uint8_t reg_data){
    uint8_t buffer[] = {reg, reg_data};
    i2c_data_t data = {0};
    data.send_buf = buffer;
    data.send_len = sizeof(buffer);
    errcode_t ret = uapi_i2c_master_write(I2C_MASTER_BUS_ID, 
INS5699S_ADDR >> 1, &data);
    if (ret != 0) {
        printf("INS5699S:I2cWriteREG(%02X) failed, %0X!\n", reg, ret);
        return ;
    }
}
uint8_t ins5699s_ReadREG(uint8_t reg){
    uint8_t send_buffer[] = {reg};
    uint8_t read_buffer[1] = {0};
    i2c_data_t data = {0};
    data.send_buf = send_buffer;
    data.send_len = sizeof(send_buffer);
    data.receive_buf = read_buffer;
    data.receive_len = 1;
    errcode_t ret = uapi_i2c_master_writeread(I2C_MASTER_BUS_ID, 
INS5699S_ADDR >> 1, &data);
    if (ret != 0) {
        printf("INS5699S:I2cReadREG(%02X) failed, %0X!\n", reg, ret);
        return 0;
    }
    return data.receive_buf[0];
}
void ins5699s_init(void){
    osal_msleep(100);
    osal_printk("INS5699S Init SUCC!\r\n");
}


ins5699s_time ins5699s_GetTime(void) {
    ins5699s_time time;
    unsigned char regData;
    // 读取秒
    regData = ins5699s_ReadREG(INS5699S_REG_SEC);
    time.sec = (regData >> 4) * 10 + (regData & 0x0F);
    // 读取分
    regData = ins5699s_ReadREG(INS5699S_REG_MIN);
    time.min = (regData >> 4) * 10 + (regData & 0x0F);
    // 读取小时
    regData = ins5699s_ReadREG(INS5699S_REG_HOUR);
    time.hour = (regData >> 4) * 10 + (regData & 0x0F);
    // 读取星期
    regData = ins5699s_ReadREG(INS5699S_REG_WEEK);
    int flag = 1;
    while(flag){
        for(int i = 0;;i++){
            if((regData >> i) & 1){
                time.week = ((regData >> i)&0x0F);
                flag = 0;
                break;  
            }
        }
    }
    time.week = ins5699s_ReadREG(INS5699S_REG_WEEK);
    // 读取日
    regData = ins5699s_ReadREG(INS5699S_REG_DAY);
    time.day = (regData >> 4) * 10 + (regData & 0x0F);
    // 读取月
    regData = ins5699s_ReadREG(INS5699S_REG_MONTH);
    time.month = (regData >> 4) * 10 + (regData & 0x0F);
    // 读取年
    regData = ins5699s_ReadREG(INS5699S_REG_YEAR);
    time.year = (regData >> 4) * 10 + (regData & 0x0F);
    return time;
}
void ins5699s_SetTime(ins5699s_time time) {
    unsigned char regData;
    //设置秒
    regData = (time.sec / 10) << 4 | (time.sec % 10);
    ins5699s_SendREG(INS5699S_REG_SEC, regData);
    //设置分
    regData = (time.min / 10) << 4 | (time.min % 10);
    ins5699s_SendREG(INS5699S_REG_MIN, regData);
    //设置小时
    regData = (time.hour / 10) << 4 | (time.hour % 10);
    ins5699s_SendREG(INS5699S_REG_HOUR, regData);
    //设置星期
    regData = 1 << (time.week - 1);
    ins5699s_SendREG(INS5699S_REG_WEEK, regData);
    //设置日
    regData = (time.day / 10) << 4 | (time.day % 10);
    ins5699s_SendREG(INS5699S_REG_DAY, regData);
    //设置月
    regData = (time.month / 10) << 4 | (time.month % 10);
    ins5699s_SendREG(INS5699S_REG_MONTH, regData);
    //设置年
    regData = (time.year / 10) << 4 | (time.year % 10);
    ins5699s_SendREG(INS5699S_REG_YEAR, regData);
}