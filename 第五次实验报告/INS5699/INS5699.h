#include "pinctrl.h"
#include "common_def.h"
#include "soc_osal.h"
#include "gpio.h"
#include "hal_gpio.h"
#include "app_init.h"
#define INS5699S_ADDR 			0x64
#define INS5699S_REG_SEC 		0x00
#define INS5699S_REG_MIN 		0x01
#define INS5699S_REG_HOUR 		0x02
#define INS5699S_REG_WEEK 	0x03
#define INS5699S_REG_DAY 		0x04
#define INS5699S_REG_MONTH 	0x05
#define INS5699S_REG_YEAR	 0x06
#define INS5699S_REG_CONTROL	 0X0F

typedef struct {
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    uint8_t week;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} ins5699s_time;

ins5699s_time ins5699s_GetTime(void);
void ins5699s_init(void);
void ins5699s_SetTime(ins5699s_time time);