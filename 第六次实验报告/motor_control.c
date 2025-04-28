#include "motor_control.h"
#include "i2c.h"
#include "osal_timer.h"
void pwm_write(uint8_t reg_data){
    uint8_t buffer[] = {reg_data};
    i2c_data_t data = {0};
    data.send_buf = buffer;
    data.send_len = sizeof(buffer);
    errcode_t ret = uapi_i2c_master_write(I2C_MASTER_BUS_ID, 0x5A, 
&data);
    if (ret != 0) {
        printf("pwm_write: failed, %0X!\n", ret);
        return ;
    }
    printf("pwm_write: success! address:0x5A\r\n");
}

void pwm_writes(uint8_t* reg_data){
    i2c_data_t data = {0};
    data.send_buf = reg_data;
    data.send_len = sizeof(reg_data);
    errcode_t ret = uapi_i2c_master_write(I2C_MASTER_BUS_ID, 0x5A, 
&data);
    if (ret != 0) {
        printf("pwm_write: failed, %0X!\n", ret);
        return ;
    }
    printf("pwm_write: success! address:0x5A\r\n");
}
void left_wheel_set(uint16_t CRR,uint16_t limit,bool dir){
    if (CRR > limit){
        CRR = limit;
    }
    uint8_t CRRH = (CRR >> 8) & 0xFF;
    uint8_t CRRL = CRR & 0xFF;
    uint8_t PWM2CH1[3] = {0};
    uint8_t PWM2CH2[3] = {0};
    if(dir){
        //dir为true,左轮前进
        PWM2CH1[0] = 0x90;
        PWM2CH1[1] = CRRH;
        PWM2CH1[2] = CRRL;

        PWM2CH2[0] = 0xA0;
        PWM2CH2[1] = 0x00;
        PWM2CH2[2] = 0x00;
    } else {
        //dir为false,左轮后退
        PWM2CH1[0] = 0x90;
        PWM2CH1[1] = 0x00;
        PWM2CH1[2] = 0x00;

        PWM2CH2[0] = 0xA0;
        PWM2CH2[1] = CRRH;
        PWM2CH2[2] = CRRL;
    }
    pwm_writes(PWM2CH1);
    pwm_writes(PWM2CH2);
}
void right_wheel_set(uint16_t CRR,uint16_t limit,bool dir){
    if(CRR > limit){
        CRR = limit;
    }
    uint8_t CRRH = (CRR >> 8) & 0xFF;
    uint8_t CRRL = CRR & 0xFF;
    uint8_t PWM1CH1[3] = {0};
    uint8_t PWM1CH2[3] = {0};
    if(dir){
        //dir为true,右轮前进
        PWM1CH1[0] = 0x70;
        PWM1CH1[1] = 0x00;
        PWM1CH1[2] = 0x00;

        PWM1CH2[0] = 0x80;
        PWM1CH2[1] = CRRH;
        PWM1CH2[2] = CRRL;
    } else {
        //dir为false,右轮后退
        PWM1CH1[0] = 0x70;
        PWM1CH1[1] = CRRH;
        PWM1CH1[2] = CRRL;

        PWM1CH2[0] = 0x80;
        PWM1CH2[1] = 0x00;
        PWM1CH2[2] = 0x00;
    }
    pwm_writes(PWM1CH1);
    pwm_writes(PWM1CH2);
}
