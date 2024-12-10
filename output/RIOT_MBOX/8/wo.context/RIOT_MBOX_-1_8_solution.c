#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <msg.h>
#include <kernel.h>
#include <evtimer.h>
#include <stdbool.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static evtimer_t my_timer;
static char stack[THREAD_STACKSIZE_MAIN];

static void _my_event_callback(evqueue_t *evqueue)
{
    msg_t msg;
    msg.content.value = 42; 
    msg_send(&msg, thread_getpid());
    printf("Event triggered and message sent with value: %d\n", msg.content.value);
}

int main(void)
{
    msg_init_queue(msg_queue, QUEUE_SIZE);

    evtimer_event_t evtimer_event;
    evtimer_msg_event_init(&evtimer_event, _my_event_callback);
    evtimer_init(&my_timer, &evtimer_event);

    evtimer_add(&my_timer, 1000 * US_PER_MS);  // Trigger every 1 second

    while (1) {
        msg_t msg;
        if (msg_receive(&msg) == 1) {
            printf("Received message with value: %d\n", msg.content.value);
        }
    }

    return 0;
}