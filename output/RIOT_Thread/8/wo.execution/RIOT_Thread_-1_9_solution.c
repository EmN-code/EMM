#include "kernel_defines.h"
#include "ztimer.h"
#include "msg.h"
#include "thread.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "xtimer.h"
#include "uint32_t.h"

#define HIGH_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define LOW_PRIORITY  (THREAD_PRIORITY_MAIN + 1)
#define STACK_SIZE    THREAD_STACKSIZE_MAIN

static char stack_one[STACK_SIZE];
static char stack_two[STACK_SIZE];

static msg_t msg_queue_one[8];
static msg_t msg_queue_two[8];

static void *low_priority_thread(void *arg) {
    msg_t msg;
    (void)arg;
    msg_init_queue(msg_queue_one, 8);

    while(1) {
        if (msg_receive(&msg)) {
            printf("Low priority thread received message with type: %u\n", msg.type);
        }
    }
    return NULL;
}

static void *high_priority_thread(void *arg) {
    (void)arg;
    msg_t message;
    message.type = 100;
    msg_init_queue(msg_queue_two, 8);

    while (1) {
        printf("High priority thread sending message with type: %u\n", message.type);
        if (msg_send(&message, thread_getpid()) < 0) {
            printf("Error sending message\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t low_pid = thread_create(stack_one, STACK_SIZE, LOW_PRIORITY, THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low_priority_thread");

    if (low_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating low priority thread\n");
        return 1;
    }

    kernel_pid_t high_pid = thread_create(stack_two, STACK_SIZE, HIGH_PRIORITY, THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high_priority_thread");

    if (high_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating high priority thread\n");
        return 1;
    }

    return 0;
}