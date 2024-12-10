
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <stdlib.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <unistd.h>
#include <stdatomic.h>
#include <MQTTClient.h>

#define QUEUE_SIZE 8
#define STACK_SIZE 512
#define PRIORITY 5

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    int i = 0;

    while (true) {
        msg.content.value = i++;
        mbox_put(&mbox, &msg);
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }
}

void receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (true) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    char stack1[STACK_SIZE];
    char stack2[STACK_SIZE];

    thread_create(stack1, STACK_SIZE, PRIORITY, 0, sender_thread, NULL, "sender");
    thread_create(stack2, STACK_SIZE, PRIORITY, 0, receiver_thread, NULL, "receiver");

    return 0;
}
