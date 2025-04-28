#include "pinctrl.h"
#include "common_def.h"
#define I2C_MASTER_BUS_ID 1
void left_wheel_set(uint16_t CRR,uint16_t limit,bool dir);
void right_wheel_set(uint16_t CRR,uint16_t limit,bool dir);
void pwm_write(uint8_t reg_data);