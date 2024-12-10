#include "msg.h"
#include "mbox.h"
#include "kernel.h"
#include "xtimer.h"
#include "timex.h"
#include <stdio.h>

#define MESSAGE_QUEUE_SIZE 8
#define EVENT_INTERVAL (2U * US_PER_SEC)

static msg_t queue[MESSAGE_QUEUE_SIZE];
static mbox_t mbox;

void *event_sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        msg_send(&msg, mbox.queue);
        printf("Event sent with value %d\n", msg.content.value);
        msg.content.value++;
        xtimer_sleep(EVENT_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    mbox_init(&mbox, queue, MESSAGE_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(event_sender_stack, sizeof(event_sender_stack),
                                            THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST,
                                            event_sender, NULL, "event_sender");

    printf("Message box application started\n");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value %d\n", msg.content.value);
    }

    return 0;
}