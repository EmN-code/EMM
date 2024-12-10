#include <stdio.h>
#include <msg.h>
#include <event/callback.h>
#include <xtimer.h>
#include <periph_conf.h>

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return -1;
    }

    // Set the desired date and time
    struct tm time = {
        .tm_year = 2024 - 1900,  // Year since 1900
        .tm_mon = 5,             // 0-11, so 5 corresponds to June
        .tm_mday = 1,            // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };
    rtc_set_time(&time);

    // Configure alarm for 5 seconds later
    xtimer_sleep(5);

    // Get the updated time
    rtc_get_time(&time);

    // Set the alarm 5 seconds from now
    time.tm_sec += 5;
    rtc_set_alarm(&time, alarm_callback, NULL);

    // Event loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}