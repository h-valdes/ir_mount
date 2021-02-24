/*
    Schematic representation of the IR mount (L-shape)
    (12)=(14)=(27)=(26)=(25)
                        ||
                        ||
                        (33)

    LEDs 26, 35, 34 are always on and set as reference 
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
#include <stdint.h>
#include <stdlib.h>

#define LED_COUNT 6
uint32_t global_led_array[LED_COUNT] = {12, 14, 27, 26, 25, 33};

typedef struct {
    int id;
    bool state;
    uint32_t *led_array;
} ir_mount_t;

ir_mount_t *IRMount_new(int start_id) {
    ir_mount_t *p_ir_mount = NULL;
    p_ir_mount = (ir_mount_t *)malloc(sizeof(ir_mount_t));
    p_ir_mount->led_array = (uint32_t *)malloc(sizeof(uint32_t) * LED_COUNT);
    p_ir_mount->id = start_id;
    p_ir_mount->state = false;

    // Process the led information
    for (int i = 0; i < LED_COUNT; i++) {
        uint32_t gpio = global_led_array[i];

        p_ir_mount->led_array[i] = gpio;

        // Init all GPIO as output and with low value
        gpio_pad_select_gpio(gpio);
        gpio_set_direction(gpio, GPIO_MODE_OUTPUT);
        gpio_set_level(gpio, 0);
    }

    return p_ir_mount;
}

void IRMount_destroy(ir_mount_t *p_ir_mount) {
    free(p_ir_mount->led_array);
    free(p_ir_mount);
}

void IRMount_turn_off(ir_mount_t *p_ir_mount) {
    for (int i = 0; i < LED_COUNT; i++) {
        uint32_t gpio = p_ir_mount->led_array[i];
        gpio_set_level(gpio, 0);
    }
}

void IRMount_turn_on(ir_mount_t *p_ir_mount) {
    uint32_t ref_gpio_0 = 33;
    uint32_t ref_gpio_1 = 25;
    uint32_t ref_gpio_2 = 12;

    // Turn on the reference gpio's 
    gpio_pad_select_gpio(ref_gpio_0);
    gpio_set_direction(ref_gpio_0, GPIO_MODE_OUTPUT); 
    gpio_set_level(ref_gpio_0, 1);

    gpio_pad_select_gpio(ref_gpio_1);
    gpio_set_direction(ref_gpio_1, GPIO_MODE_OUTPUT); 
    gpio_set_level(ref_gpio_1, 1);

    gpio_pad_select_gpio(ref_gpio_2);
    gpio_set_direction(ref_gpio_2, GPIO_MODE_OUTPUT); 
    gpio_set_level(ref_gpio_2, 1);

    int state = p_ir_mount->id;

    for (int i = 1; i < 4; i++) {
        // The ID can only be 1 or 0
        uint32_t id_gpio = p_ir_mount->led_array[LED_COUNT];
        gpio_pad_select_gpio(id_gpio);
        gpio_set_direction(id_gpio, GPIO_MODE_OUTPUT); 
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
