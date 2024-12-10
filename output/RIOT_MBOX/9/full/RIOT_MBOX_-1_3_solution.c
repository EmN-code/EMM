#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "string.h"

#define MSG_QUEUE_SIZE   (8U)
#define INTERVAL_SEC     (1U)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t message_box;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while(1) {
        msg.content.value = (kernel_pid_t)rand();
        printf("Sender Thread: Sending message with value: %u\n", msg.content.value);
        mbox_send(&message_box, &msg);
        ztimer_sleep(ZTIMER_SEC, INTERVAL_SEC);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while(1) {
        if (mbox_receive(&message_box, &msg) == 1) {
            printf("Receiver Thread: Received message with value: %u\n", msg.content.value);
        }
        ztimer_sleep(ZTIMER_SEC, INTERVAL_SEC);
    }
    return NULL;
}

int main(void) {
    printf("Starting mbox demo on ESP32\n");
    mbox_init(&message_box, msg_queue, MSG_QUEUE_SIZE);

    /* Create sender and receiver threads */
    char sender_stack[KERNEL_CONF_STACKSIZE_MAIN];
    char receiver_stack[KERNEL_CONF_STACKSIZE_MAIN];

    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender");

    thread_create(receiver_stack, sizeof(receiver_stack),
                  THREAD_PRIORITY_MAIN - 2,
                  THREAD_CREATE_STACKTEST,
                  receiver_thread, NULL, "receiver");

    return 0;
}