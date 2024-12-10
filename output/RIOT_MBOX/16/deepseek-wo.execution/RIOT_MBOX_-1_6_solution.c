
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <time_units.h>
#include <MQTTClient.h>
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
#include <arpa/inet.h>
#include <kernel_defines.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void)
{
    msg_t msg;
    int count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_put(&mbox, &msg);
        xtimer_sleep(1); // Send a message every second
    }
}

void receiver_thread(void)
{
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)sender_thread, NULL, "sender");
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)receiver_thread, NULL, "receiver");

    return 0;
}
