#include <xtimer.h>
#include <stdio.h>
#include <rtc.h>
#include <time.h>

#define ALARM_INTERVAL (5 * ZTIMER_SEC)  /* Alarm interval in seconds */

static void alarm_callback(void *arg) {
    (void)arg;  // Unused
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_set_time(&(struct tm){.tm_year=2024-1900, .tm_mon=6-1, .tm_mday=1});

    xtimer_t alarm_timer = {
        .callback = alarm_callback,
        .arg = NULL
    };

    xtimer_set(&alarm_timer, ALARM_INTERVAL);

    /* Enter main loop to keep program running */
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}