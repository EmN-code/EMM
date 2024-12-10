
#include "esp_system.h"
#include "thread.h"
#include "xtimer.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U) // 1 second in microseconds

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    xtimer_t timer;
    xtimer_init(&timer);

    while (1) {
        xtimer_set(&timer, TIMER_INTERVAL);
        event_periodic_callback(NULL);
        xtimer_wait(&timer);
    }

    return 0;
}
