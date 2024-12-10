
#include <stdio.h>
#include <ztimer.h>
#include <event/periodic.h>
#include <event/callback.h>
#include <thread.h>
#include <msg.h>

#define INTERVAL_MS (1000)  // Interval for sending events
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACK_SIZE];
static msg_t msg_queue[8];
static event_queue_t event_queue;

static void event_handler(void *arg)
{
    (void)arg;
    printf("Event received at %u ms\n", ztimer_now(ZTIMER_MSEC));
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event queue
    event_thread_init(&event_queue, stack, sizeof(stack), PRIORITY);

    // Create a periodic event callback
    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, event_handler);

    // Set the interval for the periodic event
    event_periodic_callback_set_interval(&periodic_event, INTERVAL_MS);

    // Start the periodic event
    event_periodic_callback_start(&periodic_event);

    // Main loop to handle events
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle messages if needed
    }

    return 0;
}
