#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <msg.h>
#include <evtimer.h>

#define MAIN_QUEUE_SIZE 8

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void callback_function(void *arg)
{
    (void)arg;
    printf("Event triggered and message sent\n");
    // Additional logic when event triggers can be added here
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_t evtimer;
    evtimer_msg_event_t evtimer_event;

    evtimer_event.msg.type = 0;  // Message type
    evtimer_event.msg.content.value = 0;  // Example content

    kernel_pid_t main_pid = thread_getpid();

    // Initialize the event timer with a message handler
    evtimer_init_msg(&evtimer);

    // Set the interval for event occurrences to 1 second
    evtimer_set(&evtimer.timer, US_PER_SEC, callback_function, NULL);

    while (1) {
        // Add an event to the event timer
        evtimer_add_msg(&evtimer, &evtimer_event, main_pid);
        printf("Event added to evtimer\n");

        // Sleep for 2 seconds before sending the next event
        xtimer_sleep(2);
    }

    return 0;
}