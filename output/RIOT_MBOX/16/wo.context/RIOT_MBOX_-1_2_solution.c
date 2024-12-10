#include <ztimer.h>  // Include for ztimer usage
#include <stdio.h>    // Include for standard input/output
#include <msg.h>      // Include for msg_t usage

#define MSG_QUEUE_SIZE (8)
#define EVTIMER_INTERVAL (100U)

// Message for event type
static msg_t msg_queue[MSG_QUEUE_SIZE];
static ztimer_t event_timer;

static void _evtimer_callback(void *arg)
{
    (void)arg;
    puts("Timer event triggered");

    // Placeholder to send messages - set up as needed
    // msg_send(&some_event_message, some_pid_or_receiver);
}

int main(void)
{
    // Initialize message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize ztimer with the callback function
    ztimer_set_msg(ZTIMER_USEC, &event_timer, EVTIMER_INTERVAL, NULL);

    while (1)
    {
        // Run ztimer and trigger the event
        ztimer_periodic_sleep(ZTIMER_USEC, EVTIMER_INTERVAL);
        _evtimer_callback(NULL);
    }

    return 0;
}