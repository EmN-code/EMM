
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static event_t event;

void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Event periodic callback triggered!\n");
}

int main(void)
{
    event_init(&event, event_periodic_callback, NULL);
    xtimer_init();

    while (1) {
        xtimer_periodic_wakeup((xtimer_ticks32_t *)&event, 1000000);
        event_post(&event, NULL);
    }

    return 0;
}
