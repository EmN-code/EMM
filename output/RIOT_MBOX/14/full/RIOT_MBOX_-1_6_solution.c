#include "xtimer.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "netinet/in.h"
#include "string.h"
#include "arpa/inet.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "kernel_defines.h"
#include "thread.h"
#include "msg.h"
#include "mbox.h"

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define MSG_QUEUE_SIZE (8)

static char sender_stack[STACKSIZE];
static char receiver_stack[STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t my_mbox;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 42;  // arbitrary message
        mbox_put(&my_mbox, &msg, true);
        printf("Message sent: %d\n", msg.content.value);
        xtimer_sleep(1);
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&my_mbox, &msg);
        printf("Message received: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    mbox_init(&my_mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0,
                  receiver_thread, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0,
                  sender_thread, NULL, "sender");

    return 0;
}