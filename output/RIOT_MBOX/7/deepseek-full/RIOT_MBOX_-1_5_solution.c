
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "unistd.h"
#include "stdatomic.h"
#include "MQTTClient.h"

#define QUEUE_SIZE 8
#define STACK_SIZE 512
#define PRIORITY 5

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    while (1) {
        msg.content.value = rand();
        mbox_put(&mbox, &msg);
        xtimer_usleep(1000000); // Sleep for 1 second
    }
}

void receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message: %d\n", (int)msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    char sender_stack[STACK_SIZE];
    char receiver_stack[STACK_SIZE];

    thread_create(sender_stack, STACK_SIZE, PRIORITY, 0, (thread_task_func_t)sender_thread, NULL, "sender");
    thread_create(receiver_stack, STACK_SIZE, PRIORITY, 0, (thread_task_func_t)receiver_thread, NULL, "receiver");

    return 0;
}
