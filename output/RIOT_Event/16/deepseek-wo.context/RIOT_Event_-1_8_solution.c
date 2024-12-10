
#include <stdio.h>
#include "xtimer.h"
#include "thread.h"
#include "event.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[THREAD_STACKSIZE];
static event_t periodic_event;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    puts("Starting periodic event example");

    // Initialize the periodic event
    event_init(&periodic_event, periodic_callback, NULL);

    // Create a thread to handle the periodic event
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY,
                  THREAD_CREATE_STACKTEST, periodic_callback, NULL, "periodic_thread");

    // Start the periodic event with a 1-second interval
    xtimer_periodic_event(&periodic_event, XTIMER_TICKS(1000000));

    return 0;
}
