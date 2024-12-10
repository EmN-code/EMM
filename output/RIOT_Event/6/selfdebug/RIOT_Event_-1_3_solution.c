#include <ztimer.h>
#include <stdio.h>
#include <event.h>

#define INTERVAL (1 * US_PER_SEC)

static void event_handler(void *arg)
{
    (void)arg;
    printf("Event triggered every second.\n");
}

int main(void)
{
    ztimer_periodic_t periodic_timer;
    
    printf("Initializing periodic event\n");
    ztimer_periodic_init(&periodic_timer, ZTIMER_SEC, event_handler, NULL, INTERVAL);
    ztimer_periodic_start(&periodic_timer);
    
    while (1) {
        ztimer_periodic_wakeup(ZTIMER_SEC, &periodic_timer);
    }
    return 0;
}