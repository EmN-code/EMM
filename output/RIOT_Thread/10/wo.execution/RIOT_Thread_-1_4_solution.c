#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <MQTTClient.h>

#define THREAD_STACKSIZE_MAIN    (1024)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE_SENDER  (1024)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 2)

static char stack_sender[THREAD_STACKSIZE_SENDER];

#define NUM_MSG 8
static msg_t msg_queue[NUM_MSG];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    for (uint32_t i = 0; i < 5; i++) {
        msg.content.value = i;
        printf("Sending message with value: %lu\n", (unsigned long)msg.content.value);
        msg_send(&msg, thread_getpid());
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, NUM_MSG);
    thread_create(stack_sender, sizeof(stack_sender), THREAD_PRIORITY_SENDER, 0,
                  sender_thread, NULL, "sender");

    msg_t msg, reply;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
        reply.content.value = msg.content.value + 1;
        msg_reply(&msg, &reply);
    }

    return 0;
}