#include <xtimer.h>
#include <stdio.h>
#include <event_periodic.h>

void callback(void *arg) {
    (void)arg;
    puts("Periodic event triggered");
}

int main(void) {
    evtimer_periodic_t periodic;
    evtimer_init_periodic(&periodic, callback, NULL);

    evtimer_set_periodic(&periodic, EVTIMER_PERIODIC_INTERVAL, 1000000U, 0);
    
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}