# 第六次实验报告使用说明

## 实验内容
通过 I2C 向电机驱动 IC（地址 `0x5A`）发送 PWM 控制命令，实现双轮小车的前进/后退控制。

## 文件说明

| 文件 | 说明 |
|------|------|
| `motor_control.h / .c` | 电机驱动，封装了左右轮速度和方向控制 |
| `blinky_demo.c` | 主程序，初始化 I2C 后调用电机控制函数 |

## 接口说明

```c
// 左轮设置：CRR=速度值, limit=最大限速, dir=true前进/false后退
left_wheel_set(uint16_t CRR, uint16_t limit, bool dir);

// 右轮设置（同上）
right_wheel_set(uint16_t CRR, uint16_t limit, bool dir);
```

CRR 范围建议 `0~1000`，超过 limit 会被自动截断。

## I2C 命令格式

向地址 `0x5A` 发送 3 字节：`[寄存器地址, CRR高字节, CRR低字节]`

| 寄存器 | 对应通道 |
|--------|---------|
| `0x70` | 右轮 CH1（后退方向） |
| `0x80` | 右轮 CH2（前进方向） |
| `0x90` | 左轮 CH1（前进方向） |
| `0xA0` | 左轮 CH2（后退方向） |

前进时有效通道写入 CRR 值，反向通道写 0。

## 使用步骤

1. 将 `motor_control.h`、`motor_control.c`、`blinky_demo.c` 复制到 SDK blinky app 目录。
2. 在 `CMakeLists.txt` 的 `SOURCES` 中添加这三个文件。
3. 修改 `blinky_demo.c` 中 `pwm_write(0x16)` 的初始化命令值以及 `left_wheel_set` / `right_wheel_set` 的参数来调整速度和方向。

## 注意事项

- 调用任何电机函数前需先完成 I2C 初始化（`uapi_i2c_master_init`）。
- `pwm_writes` 内部用 `sizeof(reg_data)` 计算指针长度，实际发送长度固定为 4（指针大小），如需修改发送长度请直接改为 `3`。
