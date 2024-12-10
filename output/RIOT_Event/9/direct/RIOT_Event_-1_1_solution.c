#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "periph/gpio.h"

#define INTERVAL (1U * US_PER_SEC)

static void event_periodic(void) {
    puts("Event triggered!");
}

int main(void) {
    printf("Starting RIOT application on ESP32...\n");

    while (1) {
        event_periodic();
        xtimer_sleep(1);
    }

    return 0;
}