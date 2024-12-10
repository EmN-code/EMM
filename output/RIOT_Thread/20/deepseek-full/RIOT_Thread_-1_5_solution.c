
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <timex.h>
#include <kernel_defines.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>
#include <ztimer.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <mbox.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

#define STACKSIZE 1024
#define HIGH_PRIORITY 7
#define LOW_PRIORITY 5

kernel_pid_t high_priority_pid;
kernel_pid_t low_priority_pid;

void *high_priority_thread(void *arg)
{
    msg_t msg, reply;
    while (1) {
        msg_receive(&msg);
        printf("High Priority Thread received message: %lu\n", (unsigned long)msg.content.value);
        msg_reply(&msg, &reply);
    }
    return NULL;
}

void *low_priority_thread(void *arg)
{
    msg_t msg, reply;
    while (1) {
        msg_receive(&msg);
        printf("Low Priority Thread received message: %lu\n", (unsigned long)msg.content.value);
        reply.content.value = msg.content.value + 1;
        msg_reply(&msg, &reply);
    }
    return NULL;
}

int main(void)
{
    msg_t msg, reply;

    high_priority_pid = thread_create(
        malloc(STACKSIZE),
        STACKSIZE,
        HIGH_PRIORITY,
        THREAD_CREATE_STACKTEST,
        high_priority_thread,
        NULL,
        "high_priority_thread"
    );

    low_priority_pid = thread_create(
        malloc(STACKSIZE),
        STACKSIZE,
        LOW_PRIORITY,
        THREAD_CREATE_STACKTEST,
        low_priority_thread,
        NULL,
        "low_priority_thread"
    );

    msg.type = 1;
    msg.content.value = 42;

    while (1) {
        msg_send_receive(&msg, &reply, high_priority_pid);
        printf("Main Thread received reply from High Priority Thread: %lu\n", (unsigned long)reply.content.value);

        msg.content.value = reply.content.value;
        msg_send_receive(&msg, &reply, low_priority_pid);
        printf("Main Thread received reply from Low Priority Thread: %lu\n", (unsigned long)reply.content.value);

        xtimer_sleep(1);
    }

    return 0;
}
