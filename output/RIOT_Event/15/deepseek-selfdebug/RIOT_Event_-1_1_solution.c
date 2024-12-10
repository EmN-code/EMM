
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"
#include "event/periodic.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, periodic_callback, NULL);

    /* Set the periodic event to trigger every second */
    xtimer_periodic_event(&periodic_event, 1 * US_PER_SEC);

    /* Main loop to keep the application running */
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
