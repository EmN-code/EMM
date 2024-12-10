
#include <stdlib.h>
#include <stdio.h>
#include <time_units.h>
#include <xtimer.h>
#include <thread.h>
#include <stdbool.h>
#include <unistd.h>
#include <msg.h>
#include <arpa/inet.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Event periodic callback triggered!\n");
}

int main(void)
{
    xtimer_t timer;
    msg_t msg_array[8];

    msg_init_queue(msg_array, 8);

    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        event_periodic_callback(NULL);
    }

    return 0;
}
