#include "xtimer.h"
#include "ztimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "msg.h"
#include "mbox.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "event/thread.h"

#define SENDER_THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define STACK_SIZE               (THREAD_STACKSIZE_DEFAULT)

static char sender_stack[STACK_SIZE];
static char receiver_stack[STACK_SIZE];

static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

static msg_t msg_queue[32];

void *sender_handler(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 42;

    while (true) {
        msg_send(&msg, receiver_pid);
        printf("Message sent to receiver thread\n");
        xtimer_sleep(1);
    }

    return NULL;
}

void *receiver_handler(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, 32);

    while (true) {
        msg_receive(&msg);
        printf("Message received: type=%d\n", msg.type);
    }

    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 RECEIVER_THREAD_PRIORITY, 0,
                                 receiver_handler, NULL, "receiver");

    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               SENDER_THREAD_PRIORITY, 0,
                               sender_handler, NULL, "sender");

    while (1) {
        // Main thread loop can perform other tasks
        xtimer_sleep(10);
    }

    return 0;
}