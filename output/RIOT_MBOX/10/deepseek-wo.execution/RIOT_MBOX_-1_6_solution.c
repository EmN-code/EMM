
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <stddef.h>
#include <stdatomic.h>
#include <paho_mqtt.h>
#include <stdlib.h>
#include <event/thread.h>
#include <ztimer.h>
#include <netinet/in.h>
#include <xtimer.h>
#include <msg.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <thread.h>
#include <mbox.h>
#include <stdbool.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <kernel_defines.h>

#define QUEUE_SIZE 8
#define STACK_SIZE 512

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;

    while (1) {
        xtimer_sleep(1);
        mbox_put(&mbox, &msg);
    }
}

void receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(NULL, STACK_SIZE, THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");
    thread_create(NULL, STACK_SIZE, THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}
