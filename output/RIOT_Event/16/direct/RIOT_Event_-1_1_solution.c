#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <timex.h>

#define INTERVAL_SEC    (1U)

static void event_periodic(void *arg) {
    (void)arg;
    puts("Periodic event triggered");
}

int main(void) {
    puts("RIOT ESP32: Starting event_periodic example.");

    while (1) {
        xtimer_sleep(INTERVAL_SEC);
        event_periodic(NULL);
    }

    return 0;
}