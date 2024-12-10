#include "ztimer.h"
#include "event/periodic.h"
#include "event/callback.h"

#define US_PER_SEC 1000000

void timer_callback(void *arg)
{
    (void)arg;
    printf("Callback executed every second!\n");
}

int main(void)
{
    static ztimer_periodic_t periodic_timer;
    ztimer_periodic_start(&periodic_timer, ZTIMER_MSEC, US_PER_SEC, timer_callback, NULL);
    
    while (1) {
        // Keep the main thread alive
    }
    return 0;
}