
#include "ztimer.h"
#include "mbox.h"
#include "thread.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "unistd.h"
#include "stdatomic.h"

#define QUEUE_SIZE 8
#define STACK_SIZE 256
#define PRIORITY  5

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_put(&mbox, &msg);
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }
}

void receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message: %u\n", (unsigned int)msg.content.value);
    }
}

int main(void) {
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(
        malloc(STACK_SIZE),
        STACK_SIZE,
        PRIORITY,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender"
    );

    thread_create(
        malloc(STACK_SIZE),
        STACK_SIZE,
        PRIORITY,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver"
    );

    return 0;
}
