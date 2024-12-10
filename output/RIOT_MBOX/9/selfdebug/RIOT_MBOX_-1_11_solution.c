#include <stdio.h>
#include <msg.h>
#include <evtimer_msg_event.h>
#include <evtimer.h>
#include <thread.h>

#define INTERVAL (1000000U)  // 1 second in microseconds
#define NETOPT_NONE 0

static void *thread_handler(void *arg) {
    msg_t msg;
    (void)arg;

    while (1) {
        msg_receive(&msg);  // block and wait for a message to arrive
        printf("Received event message: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void) {
    msg_t msg_queue[4];
    msg_init_queue(msg_queue, 4);

    evtimer_t evtimer;
    evtimer_msg_event_t evtimer_event;

    evtimer_init(&evtimer, NULL);
    evtimer_set_msg_event(&evtimer_event, thread_getpid(), 42);  // Arbitrary event value

    if (evtimer_add_msg(&evtimer, &evtimer_event.event, INTERVAL, EVTIMER_PERIODIC) != 0) {
        printf("Failed to add evtimer\n");
    }

    char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL, "event_thread");

    // Oversee event loop
    while (1) {
        msg_t msg;
        mbox_get(NULL, &msg, 1); // Non-blocking retrieval
        // If messages to be processed
        if (msg.content.value != NETOPT_NONE) {
            printf("Main loop received: %d\n", (int)msg.content.value);
        }
        xtimer_usleep(INTERVAL);
    }

    return 0;
}