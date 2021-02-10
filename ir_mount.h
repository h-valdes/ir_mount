#pragma once

#include <driver/gpio.h>

#define GPIO_OUTPUT_IO_0 18
#define GPIO_OUTPUT_IO_1 19
#define GPIO_OUTPUT_IO_2 20
#define GPIO_OUTPUT_IO_3 21
#define GPIO_OUTPUT_IO_4 22
#define GPIO_OUTPUT_IO_5 23

typedef void toggle_func_t(ir_mount_t *p_ir_mount);

typedef struct {
    int id;
    bool is_on;
    toggle_func_t *toggle_fn;
} ir_mount_t;

void toggle(ir_mount_t *p_ir_mount);

ir_mount_t *new_ir_mount(int id);
