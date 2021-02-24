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

// #define LED_COUNT 6
#define LED_1 14
#define LED_2 27
#define LED_3 26

#define REF_LED_1 12
#define REF_LED_2 25
#define REF_LED_3 33
// uint32_t led_array[LED_COUNT] = {12, 14, 27, 26, 25, 33};

typedef struct {
    int id;
    bool state;
} ir_mount_t;

void set_led(uint32_t led, int state) {
    gpio_pad_select_gpio(led);
    gpio_set_direction(led, GPIO_MODE_OUTPUT);
    gpio_set_level(led, state);
}

void IRMount_turn_off(ir_mount_t *p_ir_mount) {
    set_led(LED_1, 0);
    set_led(LED_2, 0);
    set_led(LED_3, 0);
    set_led(REF_LED_1, 0);
    set_led(REF_LED_2, 0);
    set_led(REF_LED_3, 0);
}

void IRMount_turn_on(ir_mount_t *p_ir_mount) {

    // Turn on the reference gpio's 
    set_led(REF_LED_1, 1);
    set_led(REF_LED_2, 1);
    set_led(REF_LED_3, 1);

    int state = p_ir_mount->id;
    switch (state) {
        case 0:
            set_led(LED_1, 0);
            set_led(LED_2, 0);
            set_led(LED_3, 0);
            break;
        case 1:
            set_led(LED_1, 1);
            set_led(LED_2, 1);
            set_led(LED_3, 1);
            break;
        default:
            break;
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

ir_mount_t *IRMount_new(int start_id) {
    ir_mount_t *p_ir_mount = NULL;
    p_ir_mount = (ir_mount_t *)malloc(sizeof(ir_mount_t));
    p_ir_mount->id = start_id;
    p_ir_mount->state = false;

    // Turn off every GPIO of the ESP32
    IRMount_turn_off(p_ir_mount);
    
    return p_ir_mount;
}

void IRMount_destroy(ir_mount_t *p_ir_mount) {
    free(p_ir_mount);
}