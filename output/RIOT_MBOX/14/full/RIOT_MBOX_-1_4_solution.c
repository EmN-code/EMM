#include "xtimer.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "event/thread.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "stdio.h"
#include "msg.h"
#include "stdlib.h"
#include "mma8x5x_params.h"
#include "stddef.h"
#include "ztimer.h"
#include "kernel_defines.h"
#include "unistd.h"
#include "evtimer_msg.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "time_units.h"
#include "mma8x5x.h"
#include "thread.h"
#include "timex.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    char sender_stack[THREAD_STACKSIZE_MAIN];
    char receiver_stack[THREAD_STACKSIZE_MAIN];
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1, 0,
                  sender_thread, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 1, 0,
                  receiver_thread, NULL, "receiver");
    return 0;
}