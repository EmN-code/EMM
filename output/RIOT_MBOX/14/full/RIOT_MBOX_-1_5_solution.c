#include "xtimer.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "time_units.h"
#include "timex.h"
#include "unistd.h"
#include "ztimer.h"
#include "dht_params.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "netinet/in.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "thread.h"
#include "arpa/inet.h"

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char sender_stack[STACKSIZE];
static char receiver_stack[STACKSIZE];

static mbox_t mbox;
static msg_t msg_queue[8];  // Message queue. Size must be a power of two.

void sender_thread(void) {
    msg_t msg;
    msg.type = 0;  // Define your message type
    msg.content.value = 1234;  // Example content

    while (1) {
        printf("Sender: Sending message...\n");
        mbox_post(&mbox, &msg, K_FOREVER);
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1000ms (1 second)
    }
}

void receiver_thread(void) {
    msg_t msg;

    while (1) {
        mbox_consume(&mbox, &msg);
        printf("Receiver: Received message with content %lu\n", (unsigned long)msg.content.value);
    }
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    mbox_init(&mbox, msg_queue, 8);

    thread_create(sender_stack, STACKSIZE, SENDER_PRIO, 0,
                  (thread_task_func_t)sender_thread, NULL, "sender");
    thread_create(receiver_stack, STACKSIZE, RECEIVER_PRIO, 0,
                  (thread_task_func_t)receiver_thread, NULL, "receiver");

    return 0;
}