#include <stdio.h>
#include "xtimer.h"
#include "periph/rtc.h"

int main(void) {
    // Initialize the RTC
    rtc_init();

    // Set the current date and time to June 1, 2024, 00:00:00
    struct tm time = { .tm_year = 2024 - 1900, // Years since 1900
                       .tm_mon  = 6 - 1,       // Months since January [0-11]
                       .tm_mday = 1,           // Day of the month [1-31]
                       .tm_hour = 0,
                       .tm_min  = 0,
                       .tm_sec  = 0 };        
    rtc_set_time(&time);

    // Schedule an alarm 5 seconds in the future
    time.tm_sec += 5;
    rtc_set_alarm(&time, alarm_callback, NULL);

    while (1) {
        // Main loop does nothing, waiting for the alarm
        xtimer_sleep(1);
    }

    return 0;
}

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}
