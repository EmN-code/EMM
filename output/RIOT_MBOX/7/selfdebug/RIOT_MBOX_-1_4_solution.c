#include "msg.h"
#include "xtimer.h"
#include "stdio.h"
#include "kernel.h"
#include "evtimer.h"
#include "event.h"

#define TIMER_INTERVAL (1000000U)  // 1 second
#define MSG_QUEUE_SIZE (8U)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_t evtimer;
static char evtimer_stack[THREAD_STACKSIZE_MAIN];

static void _evtimer_callback(void *arg) {
    msg_t msg;
    msg.type = 0x1234;
    msg.content.value = 0;  // Example value
    printf("Event timer triggered, sending message with type: %X\n", msg.type);
    msg_send(&msg, (kernel_pid_t)arg);
}

int main(void) {
    printf("RIOT OS Message Box Example\n");

    // Initialize message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize event timer for mbox events
    evtimer_init(&evtimer, _evtimer_callback);

    // Add event timer to the queue
    evtimer_msg_event_t evtimer_event;
    evtimer_event.event.callback = _evtimer_callback;
    evtimer_event.event.arg = (void *)thread_getpid();
    evtimer_event.msg.type = 0x1234;
    evtimer_event.msg.content.value = 0;  // Example value
    evtimer_add_msg(&evtimer, &evtimer_event, NULL);

    // Set a one second interval timer to configure
    xtimer_t timer;
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        // Wait for a message
        msg_t msg;
        msg_receive(&msg);
        printf("Message received with type: %X\n", msg.type);
    }

    return 0;
}