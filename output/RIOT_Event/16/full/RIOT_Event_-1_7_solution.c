/* Description: This code initializes and starts a periodic event callback function every second using RIOT OS on an ESP32. */

/* Import required libraries in descending alphabetical order */
#include <xtimer.h>
#include <stdio.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <netinet/in.h>
#include <net/emcute.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <thread.h>

/* Define a stack for the thread */
#define THREAD_STACK_SIZE (1024)
static char thread_stack[THREAD_STACK_SIZE];

/* Define the callback function called every second */
void callback_function(void *arg)
{
    (void)arg;
    puts("Callback function executed.");
}

/* Define the thread function, which initializes and starts the event timer */
void *thread_function(void *arg)
{
    (void)arg;
    /* Initialize event timer */
    evtimer_t evtimer;
    evtimer_event_t event;
    event.callback = callback_function;
    evtimer_init_msg(&evtimer);

    /* Set periodic event to 1 second */
    uint32_t period_usec = 1000000;
    while (1)
    {
        xtimer_usleep(period_usec);
        evtimer_add_msg(&evtimer, &event, NULL);
    }
    return NULL;
}

int main(void)
{
    /* Create a thread that will run the thread function */
    thread_create(thread_stack, THREAD_STACK_SIZE, THREAD_PRIORITY_MAIN - 1, 0,
                  thread_function, NULL, "event_thread");
    return 0;
}