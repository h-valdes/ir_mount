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
    bool state;
} ir_mount_t;

ir_mount_t *IRMount_new(int start_id) {
    ir_mount_t *p_ir_mount = NULL;
    p_ir_mount = malloc(sizeof(ir_mount_t));

    p_ir_mount->id = start_id;
    p_ir_mount->state = false;

    return p_ir_mount;
}

void IRMount_destroy(ir_mount_t *p_ir_mount) {
    free(p_ir_mount);
}

void IRMount_set_state(ir_mount_t *p_ir_mount_t, bool new_state) {
    p_ir_mount_t->state = new_state;
}

void IRMount_set_id(ir_mount_t *p_ir_mount_t, int new_id) {
    if ((new_id > 0) && (new_id < 7)) {
        p_ir_mount_t->id = new_id;
    }
}
