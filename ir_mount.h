#pragma once

#include <driver/gpio.h>


#define LED_COUNT 6
int led_array[LED_COUNT] = {18, 19, 20, 21, 22, 23};

typedef struct {
    int led_gpio[LED_COUNT];
    bool led_state[LED_COUNT];
} led_info_t;

typedef struct {
    int id;
    bool state;
    led_info_t led_info;
} ir_mount_t;

ir_mount_t *IRMount_new(int start_id) {
    ir_mount_t *p_ir_mount = NULL;
    p_ir_mount = malloc(sizeof(ir_mount_t));

    p_ir_mount->id = start_id;
    p_ir_mount->state = false;
    
    // Process the led information
    for (int i = 0; i < 6; i++) {
        p_ir_mount->led_info.led_gpio[i] = led_array[i];
        p_ir_mount->led_info.led_state[i] = false;

        // Init all GPIO as output and with low value
        gpio_pad_select_gpio(led_array[i]);
        gpio_set_direction(led_array[i], GPIO_MODE_OUTPUT);
        gpio_set_level(led_array[i], 0);
    }

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

