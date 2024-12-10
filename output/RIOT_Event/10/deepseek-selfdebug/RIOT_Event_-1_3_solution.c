
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void *event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered\n");
    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    xtimer_t timer;
    timer.callback = event_periodic_callback;
    timer.arg = NULL;

    xtimer_set(&timer, 1000000);  // 1 second interval

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
