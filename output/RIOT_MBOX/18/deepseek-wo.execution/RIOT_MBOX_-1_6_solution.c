
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <event/thread.h>
#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <paho_mqtt.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void)
{
    msg_t msg;
    while (1) {
        // Simulate sending an event
        msg.type = 1;
        msg.content.value = 0x1234;
        mbox_put(&mbox, &msg);
        xtimer_sleep(5); // Send event every 5 seconds
    }
}

void receiver_thread(void)
{
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg, 1); // Blocking get
        printf("Received message with type %u and value %u\n", msg.type, msg.content.value);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)sender_thread, NULL, "sender");
    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)receiver_thread, NULL, "receiver");

    return 0;
}
