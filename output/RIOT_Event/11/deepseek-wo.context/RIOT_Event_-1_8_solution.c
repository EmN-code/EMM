
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_t event;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_init(&event, periodic_callback, NULL);

    xtimer_ticks32_t interval = xtimer_ticks_from_usec(1000000); // 1 second
    event_periodic_init(&event, interval, periodic_callback, NULL);
    event_periodic_add(&event);

    return 0;
}
