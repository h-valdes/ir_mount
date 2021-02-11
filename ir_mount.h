#pragma once

#include <driver/gpio.h>

#define GPIO_OUTPUT_IO_0 18
#define GPIO_OUTPUT_IO_1 19
#define GPIO_OUTPUT_IO_2 20
#define GPIO_OUTPUT_IO_3 21
#define GPIO_OUTPUT_IO_4 22
#define GPIO_OUTPUT_IO_5 23

typedef struct {
    int id;
    bool is_on;
}ir_info_t;

typedef void set_state_func_t(bool new_state, ir_info_t *p_ir_info);
typedef void set_id_func_t(int new_id, ir_info_t *p_ir_info);

typedef struct {
    int id;
    bool is_on;
    ir_info_t *p_ir_info;
    set_state_func_t *set_state_fn;
    set_id_func_t *set_id_fn;
} ir_mount_t;

void set_state(bool new_state);
void set_id(int new_id);

ir_mount_t *IRMount_new(int id);
void IRMount_destroy();
