#include "timex.h"
#include "thread.h"
#include "event.h"
#include <stdio.h>

#define PERIOD 1U

event_periodic_t periodic_event;

void callback(void *arg) {
    (void)arg;
    puts("Periodic callback triggered.");
}

int main(void) {
    event_periodic_init(&periodic_event, &callback, NULL, PERIOD);
    puts("Starting event periodic...");
    event_periodic_start(&periodic_event);

    // Main loop does nothing, callback function will be called every second.
    while(1) {}

    return 0;
}