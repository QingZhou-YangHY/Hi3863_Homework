# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This repository contains lab report source code for a "智能硬件" (Smart Hardware) course using the **HiSilicon Hi3863** microcontroller. Code is written in C targeting HiSilicon's proprietary embedded SDK. Files here are not standalone — they must be copied into the corresponding app directory inside the full HiSilicon SDK project to compile.

## Build System

Projects use **CMake + Kconfig** as provided by the HiSilicon SDK. There is no build command runnable from this repository directly. To build:

1. Set up the full HiSilicon SDK environment (vendor-provided toolchain).
2. Copy the source files for the desired lab into the SDK's `application/samples/blinky/` (or equivalent) directory.
3. Update the app's `CMakeLists.txt` to include the new source files (see `第五次实验报告/blinky/CMakeLists.txt` for the pattern).
4. Build using the SDK's build scripts.

## SDK Conventions

All source files rely on HiSilicon SDK headers. Key APIs:

- `uapi_pin_set_mode(pin, mode)` — configure GPIO pin multiplexing
- `uapi_i2c_master_init(bus_id, baudrate, hscode)` — initialize I2C master
- `uapi_i2c_master_write(bus_id, slave_addr, &data)` — I2C write
- `osal_kthread_create(handler, arg, name, stack_size)` — create RTOS task
- `osal_kthread_set_priority(task, prio)` — set task priority
- `osal_msleep(ms)` — millisecond sleep
- `osal_printk(fmt, ...)` — kernel-level print (preferred over `printf` for sensor output)
- `app_run(entry_fn)` — macro that registers the application entry point (replaces `main`)

## Standard I2C Setup Pattern

All labs that use I2C share the same initialization pattern:

```c
// Pin 15 = SCL, Pin 16 = SDA, mode 2
uapi_pin_set_mode(15, 2);
uapi_pin_set_mode(16, 2);
uapi_i2c_master_init(1, 100000, 0x00);  // bus 1, 100kHz
```

## Task Entry Pattern

Every application registers a single RTOS task:

```c
void MyTask(void) { /* actual work */ }

void my_entry(void) {
    osal_kthread_lock();
    osal_task *taskid = osal_kthread_create((osal_kthread_handler)MyTask, NULL, "MyTask", 0x1000);
    osal_kthread_set_priority(taskid, 17);
    osal_kthread_unlock();
}

app_run(my_entry);
```

## Lab Contents

| Directory | Peripheral / Topic |
|---|---|
| `第四次实验报告/` | I2C sensors: BH1750FVI (light intensity) + CW2015 (battery voltage) |
| `第五次实验报告/blinky/` | I2C init skeleton + software timer (`timer.c`) |
| `第五次实验报告/INS5699/` | RTC chip (INS5699S) — read/write time via I2C registers |
| `第五次实验报告/timer_demo.c` | Standalone timer code for analysis (not a deployable app) |
| `第六次实验报告/` | Motor control via PWM over I2C to address `0x5A` |

## Motor Control (第六次实验报告)

The motor driver IC is controlled at I2C address `0x5A`. PWM duty is set by writing 3-byte packets:

- Byte 0: register address (`0x70`/`0x80` for right wheel, `0x90`/`0xA0` for left wheel)
- Bytes 1–2: 16-bit CRR value (high byte, low byte)

`left_wheel_set(CRR, limit, dir)` and `right_wheel_set(CRR, limit, dir)` handle direction and clamping. `dir=true` = forward.

## Commented-Out Code

Many files contain a large block of commented-out WS2812B RGB LED code at the top (the original blinky template). This is kept for reference from the original SDK sample and is not part of the active experiment.
