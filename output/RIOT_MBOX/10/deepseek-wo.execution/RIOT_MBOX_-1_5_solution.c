
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>

#define QUEUE_SIZE 8
#define THREAD_STACKSIZE 512
#define THREAD_PRIORITY_MAIN 7
#define THREAD_CREATE_STACKTEST 0

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    uint32_t count = 0;

    while (true) {
        msg.content.value = count++;
        mbox_put(&mbox, &msg);
        xtimer_sleep(1);
    }
}

void receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (true) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }
}

int main(void) {
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(
        malloc(THREAD_STACKSIZE),
        THREAD_STACKSIZE,
        THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender_thread"
    );

    thread_create(
        malloc(THREAD_STACKSIZE),
        THREAD_STACKSIZE,
        THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver_thread"
    );

    return 0;
}
