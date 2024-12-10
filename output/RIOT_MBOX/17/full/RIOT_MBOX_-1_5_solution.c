#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "netinet/in.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "arpa/inet.h"
#include "event/thread.h"
#include "thread.h"

#define MESSAGE_QUEUE_SIZE (8)

static msg_t msg_queue[MESSAGE_QUEUE_SIZE];
static char stack[THREAD_STACKSIZE_DEFAULT];
static mbox_t mbox;

void *event_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        mbox_receive(&mbox, &msg);
        printf("Message received: %ld\n", (long)msg.content.value);
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, MESSAGE_QUEUE_SIZE);

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, MESSAGE_QUEUE_SIZE);

    // Create the thread to handle events
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, event_handler, NULL, "event_handler");

    // Simulate sending messages
    for (uint32_t i = 0; i < 10; i++) {
        msg_t msg;
        msg.content.value = i;
        mbox_put(&mbox, &msg);
        printf("Message sent: %ld\n", (long)msg.content.value);
        xtimer_sleep(1);
    }
    return 0;
}