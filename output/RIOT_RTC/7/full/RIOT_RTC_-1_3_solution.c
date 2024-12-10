#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <mbox.h>
#include <stddef.h>
#include <stdatomic.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <periph/gpio.h>
#include <time_units.h>
#include <stdbool.h>
#include <periph/i2c.h>

#define ALARM_DELAY (5U)

void alarm_triggered(void *arg) {
    (void)arg; // Avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    ztimer_t timer;
    timex_t delay = timex_set(ALARM_DELAY, 0);

    while (1) {
        if (ztimer_init(ZTIMER_MSEC, &timer) != 0) {
            printf("Failed to initialize timer, retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        break;
    }

    ztimer_set_msg(&timer, ZTIMER_MSEC, &delay, alarm_triggered, NULL);
    ztimer_sleep(ZTIMER_MSEC, &delay);

    return 0;
}
