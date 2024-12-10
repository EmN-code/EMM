#include <xtimer.h>
#include <stdio.h>
#include <event/periodic.h>

#define INTERVAL_USEC (1000000U)  // 1 second in microseconds

static evtimer_t periodic_event;
static evtimer_msg_event_t event_msg;
static event_queue_t event_queue;

void periodic_callback(event_t *event)
{
    printf("Periodic event triggered\n");
}

int main(void)
{
    printf("Start periodic event example\n");

    evtimer_init_msg(&periodic_event, &event_queue, &event_msg);

    static const timex_t interval = { .seconds = 1, .microseconds = 0 };
    evtimer_add_msg(&periodic_event, &interval, NULL);

    while (1) {
        evtimer_periodic(&periodic_event);
        xtimer_usleep(INTERVAL_USEC);
    }

    return 0;
}