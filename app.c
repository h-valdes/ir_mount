#include <rcl/error_handling.h>
#include <rcl/rcl.h>
#include <rclc/executor.h>
#include <rclc/rclc.h>
#include <stdio.h>
#include <unistd.h>

#include <std_msgs/msg/bool.h>
#include <std_msgs/msg/int32.h>
#include "ir_mount.h"

#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#endif

#define RCCHECK(fn)                                                                      \
    {                                                                                    \
        rcl_ret_t temp_rc = fn;                                                          \
        if ((temp_rc != RCL_RET_OK)) {                                                   \
            printf("Failed status on line %d: %d. Aborting.\n", __LINE__, (int)temp_rc); \
            vTaskDelete(NULL);                                                           \
        }                                                                                \
    }
#define RCSOFTCHECK(fn)                                                                    \
    {                                                                                      \
        rcl_ret_t temp_rc = fn;                                                            \
        if ((temp_rc != RCL_RET_OK)) {                                                     \
            printf("Failed status on line %d: %d. Continuing.\n", __LINE__, (int)temp_rc); \
        }                                                                                  \
    }

rcl_publisher_t publisher;
rcl_subscription_t set_state_sub;
rcl_subscription_t set_id_sub;

std_msgs__msg__Bool msg;
std_msgs__msg__Bool state_msg;
std_msgs__msg__Int32 id_msg;

ir_mount_t ir_mount;

void set_state_callback(const void *msgin) {
    const std_msgs__msg__Bool *state_msg = (const std_msgs__msg__Bool *)msgin;
    ir_mount.is_on = state_msg->data;
}

void set_id_callback(const void *msgin) {
    const std_msgs__msg__Int32 *id_msg = (const std_msgs__msg__Int32 *)msgin;
}

void timer_callback(rcl_timer_t *timer, int64_t last_call_time) {
    RCLC_UNUSED(last_call_time);
    if (timer != NULL) {
        msg.data = ir_mount.is_on;
        RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
    }
}

void appMain(void *arg) {
    rcl_allocator_t allocator = rcl_get_default_allocator();
    rclc_support_t support;

    ir_mount.id = 1;
    ir_mount.is_on = true;

    // create init_options
    RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

    // create node
    rcl_node_t node;
    RCCHECK(rclc_node_init_default(&node, "ir_mount_node", "", &support));

    // create /ir_mount/is_on publisher (Bool)
    RCCHECK(rclc_publisher_init_default(
        &publisher,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "/ir_mount/is_on"));

    // create /ir_mount/set_state subscriber (Bool)
    RCCHECK(rclc_subscription_init_default(
		&set_state_sub,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
		"/ir_mount/set_state"));
    
    // create /ir_mount/set_id subscriber (Int32)
    RCCHECK(rclc_subscription_init_default(
		&set_id_sub,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
		"/ir_mount/set_id"));

    // create timer,
    rcl_timer_t timer;
    const unsigned int timer_timeout = 1000;
    RCCHECK(rclc_timer_init_default(
        &timer,
        &support,
        RCL_MS_TO_NS(timer_timeout),
        timer_callback));

    // create executor
    rclc_executor_t executor;
    RCCHECK(rclc_executor_init(
        &executor, &support.context, 2, &allocator));
    RCCHECK(rclc_executor_add_subscription(
        &executor, &set_state_sub, &state_msg, &set_state_callback, ON_NEW_DATA));
    RCCHECK(rclc_executor_add_subscription(
        &executor, &set_id_sub, &id_msg, &set_id_callback, ON_NEW_DATA));
    RCCHECK(rclc_executor_add_timer(
        &executor, &timer));

    msg.data = true;

    while (1) {
        rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));
        usleep(100000);
    }

    // free resources
    RCCHECK(rcl_publisher_fini(&publisher, &node));
    RCCHECK(rcl_subscription_fini(&set_state_sub, &node));
    RCCHECK(rcl_subscription_fini(&set_id_sub, &node));
    RCCHECK(rcl_node_fini(&node));

    vTaskDelete(NULL);
}