#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO       (THREAD_PRIORITY_MAIN - 2)
#define INTERVAL            (1U * US_PER_SEC)
#define QUEUE_SIZE          (8)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        xtimer_sleep(2);
        msg.content.value = 1;
        printf("Sending event...\n");
        mbox_put(&mbox, &msg, 0);
    }
    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received event with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    msg_init_queue(msg_queue, QUEUE_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_MAIN, SENDER_PRIO, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");
    thread_create(NULL, THREAD_STACKSIZE_MAIN, RECEIVER_PRIO, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}