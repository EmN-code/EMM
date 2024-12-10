
#include "xtimer.h"
#include "event/callback.h"
#include "event/periodic.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_periodic_t periodic_event;
    event_callback_t callback;

    callback.handler = periodic_event_callback;
    callback.arg = NULL;

    event_periodic_init(&periodic_event, &callback, 1000000); // 1 second in microseconds
    event_periodic_add(&periodic_event);

    return 0;
}
