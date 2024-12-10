
#include "mma8x5x.h"
#include "stdio.h"
#include "unistd.h"
#include "stdatomic.h"
#include "stddef.h"
#include "time_units.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "stdlib.h"
#include "event/thread.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "ztimer.h"
#include "netinet/in.h"
#include "xtimer.h"
#include "msg.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "stdbool.h"
#include "evtimer_msg.h"
#include "arpa/inet.h"
#include "kernel_defines.h"

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void)
{
    msg_t msg;
    while (1) {
        // Simulate sending events at intervals
        msg.content.value = rand();
        msg.type = 1;
        msg_send(&msg, thread_getpid(), false);
        sleep(1); // Sleep for 1 second
    }
}

void receiver_thread(void)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %u\n", msg.content.value);
    }
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Create sender and receiver threads
    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");

    return 0;
}
