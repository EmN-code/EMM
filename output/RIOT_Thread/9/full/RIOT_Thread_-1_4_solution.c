#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_DEFAULT - 1)
#define MQTT_BROKER_IP "192.168.1.10"
#define MQTT_BROKER_PORT 1883

static char thread_stack[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread_pid;

void thread_function(void *arg) {
    msg_t msg, reply;
    msg.content.value = 42;

    while (1) {
        if (msg_send_receive(&msg, &reply, thread_pid) < 0) {
            puts("Error sending message.");
        } else {
            printf("Received reply: %"PRIu32"\n", reply.content.value);
        }
        xtimer_sleep(1);
    }
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    thread_pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN, 0, thread_function, NULL, "thread");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message from thread %d\n", msg.sender_pid);

        msg_t reply = { .content.value = msg.content.value + 1 };
        msg_reply(&msg, &reply);
    }

    // MQTT Client Networking (Pseudo)
    int rc;
    Network network;
    /* Assume necessary setup for Network */
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            puts("Network connection failed, retrying...");
            xtimer_sleep(1);
            continue;
        } else {
            puts("Connected to network.");
            break;
        }
    }
    
    return 0;
}