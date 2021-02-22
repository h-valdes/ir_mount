/*
    Schematic representation of the IR mount (L-shape)
    (34)=(35)=(32)=(33)=(25)
                        ||
                        ||
                        (26)

    LEDs 34, 25 26 are always on and set as reference 
    (1)=(data)=(data)=(data)=(1)
                            ||
                            ||
                            (1)
    
    Using Triple Repetition Code for the ID Encoding with a Hamming Distance
    of 3
        ID 0 = 000 
        ID 1 = 111
*/

#pragma once

#include <driver/gpio.h>

#define LED_COUNT 6
int led_array[LED_COUNT] = {34, 35, 32, 33, 25, 26};

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
    for (int i = 0; i < LED_COUNT; i++) {
        int gpio = led_array[i];

        p_ir_mount->led_info.led_gpio[i] = gpio;
        p_ir_mount->led_info.led_state[i] = false;

        // Init all GPIO as output and with low value
        gpio_pad_select_gpio(gpio);
        gpio_set_direction(gpio, GPIO_MODE_OUTPUT);
        gpio_set_level(gpio, 0);
    }

    return p_ir_mount;
}

void IRMount_destroy(ir_mount_t *p_ir_mount) {
    free(p_ir_mount);
}

void IRMount_turn_off(ir_mount_t *p_ir_mount) {
    for (int i = 0; i < LED_COUNT; i++) {
        int gpio = p_ir_mount->led_info.led_gpio[i];
        gpio_set_level(gpio, 0);
    }
}

void IRMount_turn_on(ir_mount_t *p_ir_mount) {
    int ref_gpio_0 = p_ir_mount->led_info.led_gpio[0];
    int ref_gpio_1 = p_ir_mount->led_info.led_gpio[LED_COUNT - 2];
    int ref_gpio_2 = p_ir_mount->led_info.led_gpio[LED_COUNT - 1];

    // Turn on the reference gpio's 
    gpio_set_level(ref_gpio_0, 1);
    gpio_set_level(ref_gpio_1, 1); 
    gpio_set_level(ref_gpio_2, 1); 

    for (int i = 1; i <= 3; i++) {
        // The ID can only be 1 or 0
        int state = p_ir_mount->id; 
        int id_gpio = p_ir_mount->led_info.led_gpio[LED_COUNT];
        gpio_set_level(id_gpio, state);
    }
}

void IRMount_set_state(ir_mount_t *p_ir_mount, bool new_state) {
    p_ir_mount->state = new_state;
    if (new_state) {
        IRMount_turn_off(p_ir_mount); // Turn first every led off 
        IRMount_turn_on(p_ir_mount);
    } else {
        IRMount_turn_off(p_ir_mount);
    }
}

void IRMount_set_id(ir_mount_t *p_ir_mount, int new_id) {
    if (new_id == 0 || new_id == 1) {
        p_ir_mount->id = new_id;
    }
}
