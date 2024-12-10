#include <thread.h>
#include <stdio.h>
#include <event/periodic.h>

void callback(void *arg) {
    printf("Callback triggered: %ld\n", (long)arg);
}

int main(void) {
    evtimer_t timer;
    evtimer_periodic_event_t periodic_event;

    evtimer_init_periodic(&timer);
    evtimer_add_periodic(&timer, &periodic_event, 1000000U, callback, NULL);
    
    printf("Periodic event initialized and started.\n");

    while (1) {
        thread_yield();
    }

    return 0;
}