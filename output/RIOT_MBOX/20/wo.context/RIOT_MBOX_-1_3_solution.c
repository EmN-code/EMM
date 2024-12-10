#include "msg.h"
#include "kernel.h"
#include "xtimer.h"
#include <stdio.h>

#define MESSAGE_QUEUE_SIZE (8)
#define TIMER_DURATION (1000000) // in microseconds (1 second)

static msg_t message_queue[MESSAGE_QUEUE_SIZE];

void timeout_callback(void *arg)
{
    msg_t msg;
    msg.type = 0x1234;  // example message type
    printf("Sending message of type: %04x\n", msg.type);
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    msg_init_queue(message_queue, MESSAGE_QUEUE_SIZE);
    printf("Message queue initialized.\n");

    xtimer_t timer;
    timer.callback = timeout_callback;
    timer.arg = NULL;
    xtimer_set(&timer, TIMER_DURATION);

    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message of type: %04x\n", msg.type);
        xtimer_sleep(1);
    }

    return 0;
}