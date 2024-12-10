#include <timex.h>
#include <timer.h>
#include <stdio.h>
#include <event/periodic.h>

void callback(void *arg) {
    printf("Callback triggered\n");
}

int main() {
    evtimer_t periodic_timer;
    unsigned interval = 1 * US_PER_SEC; // 1 second interval

    evtimer_init_periodic(&periodic_timer, interval, callback, NULL);
    evtimer_set_periodic(&periodic_timer);

    while (1) {
        // Infinite loop to keep the program running
    }

    return 0;
}