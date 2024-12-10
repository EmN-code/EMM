
#include <stdio.h>
#include "xtimer.h"
#include "thread.h"
#include "msg.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    while (1) {
        periodic_callback(NULL);
        xtimer_usleep(PERIODIC_INTERVAL);
    }

    return 0;
}
