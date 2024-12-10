
#include <stdlib.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <xtimer.h>
#include <thread.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>
#include <string.h>

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void *arg) {
    msg_t msg;
    msg.type = 1;
    msg.content.value = 1234;

    while (1) {
        msg_send_receive(&msg, &msg, (kernel_pid_t)arg);
        xtimer_sleep(1);
    }
}

void receiver_thread(void *arg) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
        msg_reply(&msg, &msg);
    }
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t receiver_pid = thread_create(
        malloc(STACKSIZE),
        STACKSIZE,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver"
    );

    thread_create(
        malloc(STACKSIZE),
        STACKSIZE,
        THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        (void *)receiver_pid,
        "sender"
    );

    return 0;
}
