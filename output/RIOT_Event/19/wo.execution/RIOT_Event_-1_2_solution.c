#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include "MQTTClient.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "event/thread.h"
#include "event/periodic.h"

void my_event_callback(void *arg) {
    (void)arg;  // avoid compiler warning
    printf("Callback executed every second.\n");
}

int main(void) {
    printf("RIOT application running on ESP32\n");

    ztimer_periodic_t periodic_timer;
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, my_event_callback, NULL, 1000);
    while (1) {
        ztimer_periodic_start(ZTIMER_MSEC, &periodic_timer);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
