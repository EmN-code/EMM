#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "thread.h"
#include "msg.h"
#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "unistd.h"
#include "timex.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stddef.h"
#include "stdbool.h"
#include "string.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER  (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 1)

static char stacks[2][THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static msg_t message;

void *receiver_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, 8);
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message: %lu\n", msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0xABCD;

    while (1) {
        for (uint32_t i = 0; i < 5; i++) {
            msg.content.value = i;
            msg_send(&msg, thread_getpid());
            xtimer_sleep(1);
        }
    }
    return NULL;
}

int main(void) {
    puts("RIOT messaging example\n");

    thread_create(stacks[0], sizeof(stacks[0]), THREAD_PRIORITY_RECEIVER,
                  THREAD_CREATE_STACKTEST | THREAD_CREATE_SLEEPING,
                  receiver_thread, NULL, "receiver");

    thread_create(stacks[1], sizeof(stacks[1]), THREAD_PRIORITY_SENDER,
                  THREAD_CREATE_STACKTEST | THREAD_CREATE_SLEEPING,
                  sender_thread, NULL, "sender");

    xtimer_sleep(1);  // Allow some time before starting threads

    thread_wakeup(thread_getpid());

    return 0;
}