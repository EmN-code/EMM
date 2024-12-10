
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include <stdio.h>

#define MAIN_QUEUE_SIZE     (8)
#define EVENT_INTERVAL_MS   (1000)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static mbox_t event_mbox;
static msg_t event_queue[MAIN_QUEUE_SIZE];

void *event_sender(void *arg) {
    (void)arg;
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_put(&event_mbox, &msg);
        xtimer_sleep(EVENT_INTERVAL_MS / 1000);
    }

    return NULL;
}

void *event_receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&event_mbox, &msg, 1);
        printf("Received event with count: %lu\n", (unsigned long)msg.content.value);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    mbox_init(&event_mbox, event_queue, MAIN_QUEUE_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_receiver, NULL, "event_receiver");

    return 0;
}
