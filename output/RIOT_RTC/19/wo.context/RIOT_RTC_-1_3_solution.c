#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <event/callback.h>
#include <sys/types.h>

void rtc_alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    /* Initialize the timer with a callback function */
    event_timer_t timer;
    timer.callback = rtc_alarm_callback;
    timer.arg = NULL;

    /* Set the current time to some arbitrary value (e.g., June 1, 2024) 
       For simplicity, we'll assume it's possible to directly set such a value */
    struct tm timeinfo = { .tm_year = 2024 - 1900, .tm_mon = 6 - 1, .tm_mday = 1 };
    // This is pseudo code as the actual RTC set function may vary
    // rtc_set_time(&timeinfo);

    /* Set the timer for 5 seconds in the future */
    xtimer_set(&timer, 5 * XTIMER_SEC);

    /* Keep the process running to wait for the alarm */
    while (1) {
        // Here we ensure the main loop doesn't exit
        xtimer_sleep(10);
    }
    return 0;
}